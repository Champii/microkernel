/************************************************
*
* Micro Kernel
*
* - Standard List functs
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/
#include                  "list.h"


static inline void        __list_add(struct list_head *new,
            struct list_head *prev,
            struct list_head *next)
{
  next->prev = new;
  new->next = next;
  new->prev = prev;
  prev->next = new;
}

static inline void        list_add(struct list_head *new, struct list_head *head)
{
  __list_add(new, head, head->next);
}

static inline void        list_add_tail(struct list_head *new, struct list_head *head)
{
  __list_add(new, head->prev, head);
}

static inline void        __list_del(struct list_head *prev, struct list_head *next)
{
  next->prev = prev;
  prev->next = next;
}

static inline void        list_del(struct list_head *entry)
{
  __list_del(entry->prev, entry->next);
  entry->next = (void *) 0;
  entry->prev = (void *) 0;
}

static inline void        list_del_init(struct list_head *entry)
{
  __list_del(entry->prev, entry->next);
  INIT_LIST_HEAD(entry);
}

static inline void        list_move(struct list_head *list, struct list_head *head)
{
        __list_del(list->prev, list->next);
        list_add(list, head);
}

static inline void        list_move_tail(struct list_head *list,
          struct list_head *head)
{
        __list_del(list->prev, list->next);
        list_add_tail(list, head);
}

static inline int         list_empty(struct list_head *head)
{
  return head->next == head;
}

static inline void        __list_splice(struct list_head *list,
         struct list_head *head)
{
  struct list_head *first = list->next;
  struct list_head *last = list->prev;
  struct list_head *at = head->next;

  first->prev = head;
  head->next = first;

  last->next = at;
  at->prev = last;
}

static inline void        list_splice(struct list_head *list, struct list_head *head)
{
  if (!list_empty(list))
    __list_splice(list, head);
}

static inline void        list_splice_init(struct list_head *list,
            struct list_head *head)
{
  if (!list_empty(list)) {
    __list_splice(list, head);
    INIT_LIST_HEAD(list);
  }
}

