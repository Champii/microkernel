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
  unsigned *pid_split = (unsigned *)user_pid;
  int pid = next_pid++;

  pid_split[0] = pid;

  t_task *new_task = (t_task*)kmalloc(sizeof(t_task));
  memset(new_task, 0, sizeof(t_task));

  pid_split[1] = (unsigned)new_task;

  new_task->id = pid;

  printk(COLOR_WHITE, "Created process : ");
  printk(COLOR_WHITE, my_putnbr_base(pid, "0123456789"));
  printk(COLOR_WHITE, "\n");

  return 0;
}

int                       run_process(void *task_struct, void *entry, void *stack, void *root_pt)
{
  t_task *task = task_struct;

  task->regs.ebp = 0;
  task->regs.eip = (unsigned)entry;

  // task->regs.cs = 0x1B;
  task->regs.ds = 0x23;
  task->regs.es = 0x23;
  task->regs.fs = 0x23;
  task->regs.gs = 0x23;

  task->regs.esp = (unsigned)stack;

  // if IO service, allow in/out port
  if (task->id == IO_ID)
    task->regs.eflags = 0x1800;

  task->page_directory = root_pt;
  task->kernel_stack = (unsigned)kmalloc_a(KERNEL_STACK_SIZE);
  task->next = 0;

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
  printk(COLOR_WHITE, "Sleep : ");
  printk(COLOR_WHITE, my_putnbr_base(getpid(), "0123456789"));
  printk(COLOR_WHITE, "\n");
  current_task->sleep_count = tick + (milli / 10);

  t_task *tmp = current_task;

  switch_task(current_user_regs);
  unschedule_task(tmp);
}

