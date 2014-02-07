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

volatile t_task           *current_task;

volatile t_task           *ready_queue;

extern t_page_directory   *page_dir;
extern t_page_directory   *cur_dir;
extern void               alloc_page(t_page*,int,int);
extern unsigned           initial_esp;
extern unsigned           read_eip();

extern unsigned           next_pid;


int                       create_process(u64 *user_pid)
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

    printk(COLOR_WHITE, "Parent");

    asm volatile("sti");
    return new_task->id;
  }
  else
  {
    printk(COLOR_WHITE, "Child");
    // We are the child.
    return 0;
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

