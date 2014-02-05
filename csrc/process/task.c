/************************************************
*
* Micro Kernel
*
* - Tasks funcs
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  "task.h"
#include                  "isrs.h"
#include                  "mm.h"
#include                  "kmalloc.h"
#include                  "system.h"
#include                  "screen.h"

volatile t_task *current_task;

volatile t_task *ready_queue;

extern t_page_directory *page_dir;
extern t_page_directory *cur_dir;
extern void alloc_page(t_page*,int,int);
extern unsigned initial_esp;
extern unsigned read_eip();

unsigned next_pid = 1;

void init_tasking()
{
    asm volatile("cli");

    // Relocate the stack so we know where it is.
    // move_stack((void*)0xE0000000, 0x2000);

    // Initialise the first task (kernel task)
    // unsigned esp, ebp;
    // asm volatile("mov %%esp, %0" : "=r"(esp));
    // asm volatile("mov %%ebp, %0" : "=r"(ebp));

    current_task = ready_queue = (t_task*)kmalloc(sizeof(t_task));
    current_task->id = next_pid++;
    // current_task->esp = esp;
    // current_task->ebp = ebp;
    current_task->esp = current_task->ebp = current_task->eip = 0;
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

void switch_task(struct s_regs *regs)
{
    if (!current_task)
        return;

// return ;

    // Read esp, ebp now for saving later on.
    unsigned esp, ebp, eip;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));


    eip = read_eip();

    if (eip != 0x12345)
      printk(COLOR_BLUE, "Current process values : \n");
    else
      printk(COLOR_BLUE, "Switched process values : \n");

    printk(COLOR_BLUE, "INT NB = ");
    printk(COLOR_BLUE, my_putnbr_base(regs->int_no, "0123456789"));
    printk(COLOR_BLUE, "\n");
    printk(COLOR_BLUE, "ESP = ");
    printk(COLOR_BLUE, my_putnbr_base(regs->esp, "0123456789ABCDEF"));
    printk(COLOR_BLUE, "\n");
    printk(COLOR_BLUE, "EBP = ");
    printk(COLOR_BLUE, my_putnbr_base(regs->ebp, "0123456789ABCDEF"));
    printk(COLOR_BLUE, "\n");
    printk(COLOR_BLUE, "EIP = ");
    printk(COLOR_BLUE, my_putnbr_base(regs->eip, "0123456789ABCDEF"));
    printk(COLOR_BLUE, "\n");
    printk(COLOR_BLUE, "EIP = ");
    printk(COLOR_BLUE, my_putnbr_base(regs->eip, "0123456789ABCDEF"));
    printk(COLOR_BLUE, "\n");



    if (eip == 0x12345)
        return;

    current_task->eip = regs->eip;
    current_task->esp = regs->esp;
    current_task->ebp = regs->ebp;

    current_task = current_task->next;

    if (!current_task)
      current_task = ready_queue;

    eip = current_task->eip;
    esp = current_task->esp;
    ebp = current_task->ebp;


    cur_dir = current_task->page_directory;

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
    asm volatile("cli");

    t_task *parent_task = (t_task*)current_task;

    t_page_directory *directory = clone_directory(cur_dir);
    // for(;;);

    // printk(COLOR_WHITE, "New Directory = ");
    // printk(COLOR_WHITE, my_putnbr_base(directory, "0123456789ABCDEF"));
    // printk(COLOR_WHITE, "\n");

    t_task *new_task = (t_task*)kmalloc(sizeof(t_task));

    new_task->id = next_pid++;
    new_task->esp = new_task->ebp = 0;
    new_task->eip = 0;
    new_task->page_directory = directory;
    new_task->next = 0;

    t_task *tmp_task = (t_task*)ready_queue;
    while (tmp_task->next)
        tmp_task = tmp_task->next;
    tmp_task->next = new_task;

    unsigned eip = read_eip();

    // if (current_task != parent_task)
    //   printk(COLOR_WHITE, "Pre-Child");

    if (current_task == parent_task)
    {
        unsigned esp; asm volatile("mov %%esp, %0" : "=r"(esp));
        unsigned ebp; asm volatile("mov %%ebp, %0" : "=r"(ebp));
        new_task->esp = esp;
        new_task->ebp = ebp;
        new_task->eip = eip;
        printk(COLOR_BLUE, "Saved values on fork : \n");
        printk(COLOR_BLUE, "ESP = ");
        printk(COLOR_BLUE, my_putnbr_base(esp, "0123456789ABCDEF"));
        printk(COLOR_BLUE, "\n");
        printk(COLOR_BLUE, "EBP = ");
        printk(COLOR_BLUE, my_putnbr_base(ebp, "0123456789ABCDEF"));
        printk(COLOR_BLUE, "\n");
        printk(COLOR_BLUE, "EIP = ");
        printk(COLOR_BLUE, my_putnbr_base(eip, "0123456789ABCDEF"));
        printk(COLOR_BLUE, "\n");
        asm volatile("sti");

        printk(COLOR_WHITE, "Parent");

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
