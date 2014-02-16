/************************************************
*
* Micro Kernel
*
* - Processes funcs
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  "task.h"
#include                  "process.h"
#include                  "screen.h"
#include                  "kmalloc.h"
#include                  "system.h"

extern t_task             *current_task;

extern t_task             *ready_queue;
extern t_task             *wait_queue;

extern t_page_directory   *cur_dir;
extern unsigned           initial_esp;

extern unsigned           next_pid;

extern u64                *pl_pid;
extern u64                *paging_pid;
extern u64                *io_pid;

extern unsigned           tick;

extern struct s_regs      *current_user_regs;

int                       create_process(u64 *user_pid)
{
  unsigned *lol = (unsigned *)user_pid;
  int pid = next_pid++;

  lol[0] = pid;

  t_task *new_task = (t_task*)kmalloc(sizeof(t_task));
  memset(new_task, 0, sizeof(t_task));

  lol[1] = (unsigned)new_task;

  new_task->id = pid;

  printk(COLOR_WHITE, "Created process : ");
  printk(COLOR_WHITE, my_putnbr_base(pid, "0123456789"));
  printk(COLOR_WHITE, "\n");

  return 0;
}

static void               push_pid_on_stack(unsigned **stack, u64 *pid)
{
  unsigned *pid_split = (unsigned *)pid;
  **stack = pid_split[1];
  *stack -= 1;
  **stack = pid_split[0];
  *stack -= 1;
}

static void               prepare_stack(t_task *task, void **stack)
{
  unsigned *ustack = *stack;

  // Push pl_pid on stack
  switch_page_directory(task->page_directory);

  // if Program Loader, push all services pid on stack (reverse order)
  if (task->id == 1)
  {
    // printk(COLOR_WHITE, "STACK = ");
    // printk(COLOR_WHITE, my_putnbr_base((unsigned)stack, "0123456789ABCDEF"));
    // printk(COLOR_WHITE, "\n");
    push_pid_on_stack(&ustack, io_pid);
    // printk(COLOR_WHITE, "STACK = ");
    // printk(COLOR_WHITE, my_putnbr_base((unsigned)stack, "0123456789ABCDEF"));
    // printk(COLOR_WHITE, "\n");
    push_pid_on_stack(&ustack, paging_pid);
  }

  push_pid_on_stack(&ustack, pl_pid);
    // printk(COLOR_WHITE, "STACK = ");
    // printk(COLOR_WHITE, my_putnbr_base((unsigned)stack, "0123456789ABCDEF"));
    // printk(COLOR_WHITE, "\n");

  task->regs.esp = (unsigned)ustack;

  // if (task->id == 1)
  // {

  // printk(COLOR_WHITE, "stack[0] : 0x");
  // printk(COLOR_WHITE, my_putnbr_base(*ustack, "0123456789ABCDEF"));
  // printk(COLOR_WHITE, "\n");
  // printk(COLOR_WHITE, "stack[1] : 0x");
  // printk(COLOR_WHITE, my_putnbr_base(*(ustack + 1), "0123456789ABCDEF"));
  // printk(COLOR_WHITE, "\n");
  // printk(COLOR_WHITE, "stack[2] : 0x");
  // printk(COLOR_WHITE, my_putnbr_base(*(ustack + 2), "0123456789ABCDEF"));
  // printk(COLOR_WHITE, "\n");
  // printk(COLOR_WHITE, "stack[3] : 0x");
  // printk(COLOR_WHITE, my_putnbr_base(*(ustack + 3), "0123456789ABCDEF"));
  // printk(COLOR_WHITE, "\n");
  // printk(COLOR_WHITE, "stack[4] : 0x");
  // printk(COLOR_WHITE, my_putnbr_base(*(ustack + 4), "0123456789ABCDEF"));
  // printk(COLOR_WHITE, "\n");
  // printk(COLOR_WHITE, "stack[5] : 0x");
  // printk(COLOR_WHITE, my_putnbr_base(*(ustack + 5), "0123456789ABCDEF"));
  // printk(COLOR_WHITE, "\n");
  // printk(COLOR_WHITE, "stack[6] : 0x");
  // printk(COLOR_WHITE, my_putnbr_base(*(ustack + 6), "0123456789ABCDEF"));
  // printk(COLOR_WHITE, "\n");
  // }

  switch_page_directory(cur_dir);
}

int                       run_process(void *task_struct, void *entry, void *stack, void *root_pt)
{
  t_task *task = task_struct;

  task->regs.ebp = 0;
  task->regs.eip = (unsigned)entry;
  task->page_directory = root_pt;
  task->kernel_stack = (unsigned)kmalloc_a(KERNEL_STACK_SIZE);
  task->next = 0;

  prepare_stack(task, &stack);

  schedule_task(task);


  printk(COLOR_WHITE, "Running process : ");
  printk(COLOR_WHITE, my_putnbr_base(task->id, "0123456789"));
  printk(COLOR_WHITE, "\n");

  return 0;
}

int                       kill(u64 pid)
{

  return 0;
}

int                       wait(u64 pid)
{

  return 0;
}

void                      sleep(u32 milli)
{
  current_task->sleep_count = tick + (milli / 10);

  t_task *tmp = current_task;

  switch_task(current_user_regs);
  unschedule_task(tmp);
}

