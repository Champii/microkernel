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


int                       create_process(u32 *user_pid)
{
  asm volatile("cli");
  struct s_regs *regs = (struct s_regs *)user_pid;

  t_task *parent_task = (t_task*)current_task;

  t_page_directory *directory = clone_directory(cur_dir);

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

  if (current_task == parent_task)
  {
    memcpy(&new_task->regs, regs, sizeof(*regs));

    asm volatile("sti");

    return new_task->id;
  }

  return 0;
}

int                       run_process(void *task_struct, void *entry, void *stack, void *root_pt)
{

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

