/************************************************
*
* Micro Kernel
*
* - Ipc funcs
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  "ipc.h"
#include                  "screen.h"
#include                  "process.h"
#include                  "task.h"
#include                  "mm.h"
#include                  "kmalloc.h"
#include                  "system.h"

extern t_page_directory   *cur_dir;

extern t_task             *ready_queue;
extern t_task             *wait_queue;
extern t_task             *current_task;

u64                       get_pid_of(t_task *task)
{
  u64                     res;
  unsigned                *pid_split = (unsigned *)&res;

  pid_split[0] = task->id;
  pid_split[1] = (unsigned)task;

  return (res);
}

t_task                    *get_task_of(u64 pid)
{
  t_task                  *res;
  unsigned                *pid_split = (unsigned *)&pid;

  res = (t_task *)pid_split[1];

  return (res);
}

int                       queue_mess(u64 pid, void *msg, unsigned size)
{
  unsigned                *pid_split = (unsigned *)&pid;
  t_task                  *task = (t_task *)pid_split[1];
  t_mess                  *tmp = task->mess_queue;

  t_mess                  *new_mess = kmalloc(sizeof(*new_mess));
  char                    *msg_str = kmalloc(size);

  memset(new_mess, 0, sizeof(*new_mess));

  new_mess->sender = get_pid_of(current_task);
  memcpy(msg_str, msg, size);
  new_mess->msg = msg_str;
  new_mess->size = size;
  new_mess->next = 0;

  if (tmp)
  {
    while (tmp->next)
      tmp = tmp->next;

    tmp->next = new_mess;
  }
  else
    task->mess_queue = new_mess;

  // printk(COLOR_WHITE, "\nSEND : 0x");
  // printk(COLOR_WHITE, my_putnbr_base(pid_split[0], "0123456789ABCDEF"));
  // printk(COLOR_WHITE, " Task = 0x");
  // printk(COLOR_WHITE, my_putnbr_base(pid_split[1], "0123456789ABCDEF"));
  // printk(COLOR_WHITE, " msg = 0x");
  // printk(COLOR_WHITE, my_putnbr_base((unsigned)msg, "0123456789ABCDEF"));
  // printk(COLOR_WHITE, " = ");
  // printk(COLOR_WHITE, (char *)msg);
  // printk(COLOR_WHITE, " func id = ");
  // printk(COLOR_WHITE, my_putnbr_base(*(unsigned *)msg, "0123456789"));
  // printk(COLOR_WHITE, "\n");

  return 0;
}

int                       send(u64 to, void *msg, unsigned size)
{

  return queue_mess(to, msg, size);
}

void                      map_ipc_page(unsigned address, t_task *from, t_task *to)
{
  t_page *page_from = get_page(address, 0, from->page_directory);
  t_page *page_to = get_page(IPC_PAGE_1_ADDR, 1, to->page_directory);

  memcpy(page_to, page_from, sizeof(t_page));

  address += 1000;

  page_from = get_page(address, 0, from->page_directory);
  page_to = get_page(IPC_PAGE_2_ADDR, 1, to->page_directory);

  memcpy(page_to, page_from, sizeof(t_page));

  // refresh cr3
  switch_page_directory(to->page_directory);

}

void                      umap_ipc_page(t_task *from)
{
  memset(&from->page_directory->tables[IPC_TABLE_1_IDX]->pages[IPC_PAGE_1_IDX], 0, sizeof(t_page));

  memset(&from->page_directory->tables[IPC_TABLE_2_IDX]->pages[IPC_PAGE_2_IDX], 0, sizeof(t_page));

  // refresh cr3
  switch_page_directory(from->page_directory);
}

int                       recv(u64 pid, void *pool, unsigned pool_size)
{
  t_mess                  *top = current_task->mess_queue;


  if (!top)
  {
    // printk(COLOR_WHITE, "MSGQUEUE EMPTY for : ");
    // printk(COLOR_WHITE, my_putnbr_base(current_task->id, "0123456789"));
    // printk(COLOR_WHITE, "\n");
    return 0;
  }

  //mmap
  map_ipc_page((unsigned)top->msg, get_task_of(top->sender), current_task);

  //copy send -> recv
  unsigned offset = (unsigned)top->msg % 0x1000;
  void *tmp = (void *)(IPC_PAGE_1_ADDR + offset);
  memcpy(pool, tmp, top->size);

  //ummap
  umap_ipc_page(current_task);

  memcpy(((char *)pool) + top->size, &top->sender, sizeof(u64));

  current_task->mess_queue = current_task->mess_queue->next;

  // switch_page_directory(current_task->page_directory);
  // unsigned                *pid_split = (unsigned *)&pid;

  // printk(COLOR_WHITE, "RECV : 0x");
  // printk(COLOR_WHITE, my_putnbr_base(getpid(), "0123456789ABCDEF"));
  // printk(COLOR_WHITE, " msg = 0x");
  // printk(COLOR_WHITE, my_putnbr_base((unsigned)pool, "0123456789ABCDEF"));
  // printk(COLOR_WHITE, " size = ");
  // printk(COLOR_WHITE, my_putnbr_base(pool_size, "0123456789"));
  // printk(COLOR_WHITE, " func id = ");
  // printk(COLOR_WHITE, my_putnbr_base(*(unsigned *)pool, "0123456789"));
  // printk(COLOR_WHITE, " = ");
  // printk(COLOR_WHITE, " GOT = ");
  // printk(COLOR_WHITE, my_putnbr_base(top->size, "0123456789"));
  // printk(COLOR_WHITE, "\n");

  // int i;
  // char c[2];
  // c[1] = 0;
  // for (i = 4; i < top->size; i++)
  // {
  //   c[0] = ((char *)pool)[i];
  //   printk(COLOR_WHITE, c);
  // }
  // printk(COLOR_WHITE, "\n");

  // printk(COLOR_WHITE, "RETURN RECV");
  // printk(COLOR_WHITE, my_putnbr_base(top->size, "0123456789"));
  // printk(COLOR_WHITE, "\n");

  return (top->size);
}

