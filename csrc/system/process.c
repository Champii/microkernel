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

extern unsigned           tick;

int                       create_process(u64 *user_pid)
{
  unsigned *lol = (unsigned *)user_pid;
  int pid = next_pid++;

  lol[0] = pid;

  t_task *new_task = (t_task*)kmalloc(sizeof(t_task));
  memset(new_task, 0, sizeof(t_task));

  lol[1] = (unsigned)new_task;

  new_task->id = pid;

  printk(COLOR_WHITE, "Created process ! :");
  printk(COLOR_WHITE, my_putnbr_base(pid, "0123456789"));
  printk(COLOR_WHITE, "\n");

  return 0;
}

int                       run_process(void *task_struct, void *entry, void *stack, void *root_pt)
{
  t_task *task = task_struct;

  task->regs.ebp = 0;
  // task->regs.esp = (unsigned)stack;
  task->regs.eip = (unsigned)entry;
  task->page_directory = root_pt;
  task->kernel_stack = (unsigned)kmalloc_a(KERNEL_STACK_SIZE);
  task->next = 0;

  // Push pl_pid on stack
  switch_page_directory(task->page_directory);

  unsigned *pid_split = (unsigned *)pl_pid;
  unsigned *ustack = stack;
  *ustack = pid_split[1];
  ustack -= 1;
  *ustack = pid_split[0];
  ustack -= 1;
  task->regs.esp = (unsigned)ustack;

  // printk(COLOR_WHITE, "stack[0] : 0x");
  // printk(COLOR_WHITE, my_putnbr_base(*ustack, "0123456789ABCDEF"));
  // printk(COLOR_WHITE, "\n");
  // printk(COLOR_WHITE, "stack[1] : 0x");
  // printk(COLOR_WHITE, my_putnbr_base(*(ustack + 1), "0123456789ABCDEF"));
  // printk(COLOR_WHITE, "\n");

  switch_page_directory(cur_dir);

  schedule_task(task);

  // t_task *tmp_task = (t_task*)ready_queue;

  // if (!tmp_task)
  //   tmp_task = ready_queue = task;
  // else
  // {
  //   while (tmp_task->next)
  //     tmp_task = tmp_task->next;

  //   tmp_task->next = task;

  // }

  printk(COLOR_WHITE, "Running process ! ");
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
  // printk(COLOR_WHITE, "SLEEP KERNELSIDE !\n");
  current_task->sleep_count = tick + (milli / 10);

  unschedule_task(current_task);
  // switch_task(&current_task->regs);
}

