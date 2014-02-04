//
// task.c - Implements the functionality needed to multitask.
//          Written for JamesM's kernel development tutorials.
//

#include                  "task.h"
#include                  "mm.h"
#include                  "kmalloc.h"
#include                  "system.h"
#include                  "screen.h"

// The currently running task.
volatile t_task *current_task;

// The start of the task linked list.
volatile t_task *ready_queue;

// Some externs are needed to access members in paging.c...
extern t_page_directory *page_dir;
extern t_page_directory *cur_dir;
extern void alloc_page(t_page*,int,int);
extern unsigned initial_esp;
extern unsigned read_eip();

// The next available process ID.
unsigned next_pid = 1;

void init_tasking()
{
    // Rather important stuff happening, no interrupts please!
    asm volatile("cli");

    // Relocate the stack so we know where it is.
    move_stack((void*)0xE0000000, 0x2000);

    // Initialise the first task (kernel task)
    current_task = ready_queue = (t_task*)kmalloc(sizeof(t_task));
    current_task->id = next_pid++;
    current_task->esp = current_task->ebp = 0;
    current_task->eip = 0;
    current_task->page_directory = cur_dir;
    current_task->next = 0;

    // Reenable interrupts.
    asm volatile("sti");
}

void move_stack(void *new_stack_start, unsigned size)
{
  unsigned i;
  // Allocate some space for the new stack.
  for( i = (unsigned)new_stack_start;
       i >= ((unsigned)new_stack_start-size);
       i -= 0x1000)
  {
    // General-purpose stack is in user-mode.
    alloc_page( get_page(i, 1, cur_dir), 0 /* User mode */, 1 /* Is writable */ );
  }

  // Flush the TLB by reading and writing the page directory address again.
  unsigned pd_addr;
  asm volatile("mov %%cr3, %0" : "=r" (pd_addr));
  asm volatile("mov %0, %%cr3" : : "r" (pd_addr));

  // Old ESP and EBP, read from registers.
  unsigned old_stack_pointer; asm volatile("mov %%esp, %0" : "=r" (old_stack_pointer));
  unsigned old_base_pointer;  asm volatile("mov %%ebp, %0" : "=r" (old_base_pointer));

  // Offset to add to old stack addresses to get a new stack address.
  unsigned offset            = (unsigned)new_stack_start - initial_esp;

  // New ESP and EBP.
  unsigned new_stack_pointer = old_stack_pointer + offset;
  unsigned new_base_pointer  = old_base_pointer  + offset;

  // Copy the stack.
  memcpy((void*)new_stack_pointer, (void*)old_stack_pointer, initial_esp-old_stack_pointer);

  // Backtrace through the original stack, copying new values into
  // the new stack.
  for(i = (unsigned)new_stack_start; i > (unsigned)new_stack_start-size; i -= 4)
  {
    unsigned tmp = * (unsigned*)i;
    // If the value of tmp is inside the range of the old stack, assume it is a base pointer
    // and remap it. This will unfortunately remap ANY value in this range, whether they are
    // base pointers or not.
    if (( old_stack_pointer < tmp) && (tmp < initial_esp))
    {
      tmp = tmp + offset;
      unsigned *tmp2 = (unsigned*)i;
      *tmp2 = tmp;
    }
  }

  // Change stacks.
  asm volatile("mov %0, %%esp" : : "r" (new_stack_pointer));
  asm volatile("mov %0, %%ebp" : : "r" (new_base_pointer));
}

void switch_task()
{
    // If we haven't initialised tasking yet, just return.
    if (!current_task)
        return;

    // Read esp, ebp now for saving later on.
    unsigned esp, ebp, eip;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));

    printk(COLOR_BLUE, "ESP = ");
    printk(COLOR_BLUE, my_putnbr_base(esp, "0123456789ABCDEF"));
    printk(COLOR_BLUE, "\n");
    printk(COLOR_BLUE, "EBP = ");
    printk(COLOR_BLUE, my_putnbr_base(ebp, "0123456789ABCDEF"));
    printk(COLOR_BLUE, "\n");

    // Read the instruction pointer. We do some cunning logic here:
    // One of two things could have happened when this function exits -
    //   (a) We called the function and it returned the EIP as requested.
    //   (b) We have just switched tasks, and because the saved EIP is essentially
    //       the instruction after read_eip(), it will seem as if read_eip has just
    //       returned.
    // In the second case we need to return immediately. To detect it we put a dummy
    // value in EAX further down at the end of this function. As C returns values in EAX,
    // it will look like the return value is this dummy value! (0x12345).
    eip = read_eip();
    printk(COLOR_BLUE, "EIP = ");
    printk(COLOR_BLUE, my_putnbr_base(eip, "0123456789ABCDEF"));
    printk(COLOR_BLUE, "\n");

    // Have we just switched tasks?
    if (eip == 0x12345)
        return;

    // No, we didn't switch tasks. Let's save some register values and switch.
    current_task->eip = eip;
    current_task->esp = esp;
    current_task->ebp = ebp;

    // Get the next task to run.
    current_task = current_task->next;
    // If we fell off the end of the linked list start again at the beginning.
    if (!current_task)
      current_task = ready_queue;

    eip = current_task->eip;
    esp = current_task->esp;
    ebp = current_task->ebp;

    // Make sure the memory manager knows we've changed page directory.
    cur_dir = current_task->page_directory;
    // Here we:
    // * Stop interrupts so we don't get interrupted.
    // * Temporarily puts the new EIP location in ECX.
    // * Loads the stack and base pointers from the new task struct.
    // * Changes page directory to the physical address (physicalAddr) of the new directory.
    // * Puts a dummy value (0x12345) in EAX so that above we can recognise that we've just
    //   switched task.
    // * Restarts interrupts. The STI instruction has a delay - it doesn't take effect until after
    //   the next instruction.
    // * Jumps to the location in ECX (remember we put the new EIP in there).
    asm volatile("         \
      cli;                 \
      mov %0, %%ecx;       \
      mov %1, %%esp;       \
      mov %2, %%ebp;       \
      mov %3, %%cr3;       \
      mov $0x12345, %%eax; \
      sti;                 \
      jmp *%%ecx           "
                 : : "r"(eip), "r"(esp), "r"(ebp), "r"(cur_dir->physicalAddr));
}

int fork()
{
    // We are modifying kernel structures, and so cannot
    asm volatile("cli");

    // Take a pointer to this process' task struct for later reference.
    t_task *parent_task = (t_task*)current_task;

    // Clone the address space.
    t_page_directory *directory = clone_directory(cur_dir);
    // for(;;);

    // printk(COLOR_WHITE, "New Directory = ");
    // printk(COLOR_WHITE, my_putnbr_base(directory, "0123456789ABCDEF"));
    // printk(COLOR_WHITE, "\n");

    // Create a new process.
    t_task *new_task = (t_task*)kmalloc(sizeof(t_task));

    new_task->id = next_pid++;
    new_task->esp = new_task->ebp = 0;
    new_task->eip = 0;
    new_task->page_directory = directory;
    new_task->next = 0;

    // Add it to the end of the ready queue.
    t_task *tmp_task = (t_task*)ready_queue;
    while (tmp_task->next)
        tmp_task = tmp_task->next;
    tmp_task->next = new_task;

    // This will be the entry point for the new process.
    unsigned eip = read_eip();


    // We could be the parent or the child here - check.
    if (current_task == parent_task)
    {
        // We are the parent, so set up the esp/ebp/eip for our child.
        unsigned esp; asm volatile("mov %%esp, %0" : "=r"(esp));
        unsigned ebp; asm volatile("mov %%ebp, %0" : "=r"(ebp));
        new_task->esp = esp;
        new_task->ebp = ebp;
        new_task->eip = eip;
        asm volatile("sti");

        // printk(COLOR_WHITE, "Parent");

        return new_task->id;
    }
    else
    {
        printk(COLOR_WHITE, "Child");
        // We are the child.
        return 0;
    }

}

int getpid()
{
    return current_task->id;
}
