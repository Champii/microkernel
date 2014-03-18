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
#include                  "gdt.h"
#include                  "kmalloc.h"
#include                  "system.h"
#include                  "screen.h"

t_task *current_task;

t_task *ready_queue = 0;
t_task *wait_queue = 0;

extern t_page_directory *page_dir;
extern t_page_directory *cur_dir;
extern unsigned initial_esp;
extern unsigned read_eip();

extern unsigned tick;

struct s_regs *current_user_regs = 0;

unsigned next_pid = 1;

void init_tasking()
{

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

void switch_to_user_mode()
{
   // Set up a stack structure for switching to user mode.

  if (!ready_queue)
  {
    printk(COLOR_WHITE, "No process to launch ! Exiting.\n");
    for (;;);
  }

  set_kernel_stack(ready_queue->kernel_stack + KERNEL_STACK_SIZE);

  current_task = ready_queue;

  switch_page_directory(ready_queue->page_directory);

   asm volatile ("  \
     mov $0x23, %%ax; \
     mov %%ax, %%ds; \
     mov %%ax, %%es; \
     mov %%ax, %%fs; \
     mov %%ax, %%gs; \
                   \
     mov %0, %%eax; \
     pushl $0x23; \
     pushl %%eax; \
     pushf; \
     pop %%eax; \
     or $0x200, %%eax; \
     push %%eax;   \
     pushl $0x1B; \
     mov %1, %%eax; \
     push %%eax; \
     iret; \
     "
     : : "c"(ready_queue->regs.esp), "d"(ready_queue->regs.eip));

}

t_task *get_last_of(t_task *list)
{
  t_task *tmp = list;

  if (!tmp)
    return (0);

  while (tmp->next)
    tmp = tmp->next;

  return (tmp);
}

void add_to_end_of(t_task **list, t_task *item)
{
  t_task *tmp = get_last_of(*list);

  if (tmp)
    tmp->next = item;
  else
    *list = item;

  item->next = 0;
}

void remove_from(t_task **list, t_task *item)
{
  t_task *tmp = *list;

  if (tmp == item)
    *list = tmp->next;
  else
  {
    while (tmp->next != item)
      tmp = tmp->next;

    if (!tmp->next)
      printk(COLOR_WHITE, "NO TASK TO REMOVE TO\n");
    tmp->next = item->next;
  }
}

void schedule_task(t_task *task)
{
  add_to_end_of(&ready_queue, task);
}

void reschedule_task(t_task *task)
{
  remove_from(&wait_queue, task);

  schedule_task(task);
}

void unschedule_task(t_task *task)
{
  remove_from(&ready_queue, task);

  add_to_end_of(&wait_queue, task);
}

void check_sleeping()
{
  t_task *tmp = wait_queue;

  while (tmp)
  {
    t_task *tmp_next = tmp->next;

    if (tmp->sleep_count < tick)
      reschedule_task(tmp);

    tmp = tmp_next;
  }
}

void switch_task(struct s_regs *regs)
{
  if (!current_task)
    return;

  check_sleeping();

  memcpy(&current_task->regs, regs, sizeof (*regs));

  current_task = current_task->next;

  if (!current_task)
    current_task = ready_queue;

  if (!current_task->regs.cs)
  {
    unsigned eip = current_task->regs.eip;
    unsigned esp = current_task->regs.esp;
    unsigned ebp = current_task->regs.ebp;
    unsigned eflags = current_task->regs.eflags;

    memcpy(&current_task->regs, regs, sizeof (*regs));
    current_task->regs.eip = eip;
    current_task->regs.useresp = esp;
    current_task->regs.ebp = ebp;
    current_task->regs.eflags |= eflags;
  }

  memcpy(regs, &current_task->regs, sizeof (*regs));

  set_kernel_stack(current_task->kernel_stack + KERNEL_STACK_SIZE);
  cur_dir = current_task->page_directory;
  switch_page_directory(current_task->page_directory);

}

int getpid()
{
    return current_task->id;
}
