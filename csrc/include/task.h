/************************************************
*
* Micro Kernel
*
* - GDT main init funcs header
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#ifndef               __TASK_H__
# define              __TASK_H__

# include             "mm.h"
# include             "isrs.h"

# define              KERNEL_STACK_SIZE 2048

// This structure defines a 'task' - a process.
typedef struct        task
{
  int                 id;               // Process ID.
  // unsigned            esp, ebp;         // Stack and base pointers.
  // unsigned            eip;              // Instruction pointer.
  struct s_regs       regs;
  unsigned            kernel_stack;
  t_page_directory    *page_directory;  // Page directory.
  struct task         *next;            // The next task in a linked list.
}                     t_task;

// Initialises the tasking system.
void                  init_tasking();

// Called by the timer hook, this changes the running process.
void                  switch_task(struct s_regs *regs);

// Forks the current process, spawning a new one with a different
// memory space.
int                   fork();

// Causes the current process' stack to be forcibly moved to a new location.
void                  move_stack(void *new_stack_start, unsigned size);

// Returns the pid of the current process.
int                   getpid();

void                  switch_to_user_mode();

#endif
