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

volatile t_task           *current_task;

volatile t_task           *ready_queue;

extern t_page_directory   *cur_dir;
extern unsigned           initial_esp;

extern unsigned           next_pid;


int                       create_process(u64 *user_pid)
{
  unsigned *lol = (unsigned *)user_pid;
  int pid = next_pid++;

  lol[0] = pid;

  t_task *new_task = (t_task*)kmalloc(sizeof(t_task));
  memset(new_task, 0, sizeof(t_task));

  lol[1] = (unsigned)new_task;

  new_task->id = pid;


  return 0;
}

int                       run_process(void *task_struct, void *entry, void *stack, void *root_pt)
{
  printk(COLOR_WHITE, "Running process !\n");

  t_task *task = task_struct;

  // task->regs.esp = (unsigned)stack + 50;
  // task->regs.ebp = (unsigned)stack + 50;
  task->regs.eip = (unsigned)entry;
  task->page_directory = root_pt;
  // task->page_directory = cur_dir;

  t_task *tmp_task = (t_task*)ready_queue;

  while (tmp_task->next)
    tmp_task = tmp_task->next;

  tmp_task->next = task;


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

}

