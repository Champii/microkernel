/************************************************
*
* Micro Kernel
*
* - Standard List functs header
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/
#ifndef                     __LIST_H__
# define                    __LIST_H__

struct                      list_head {
  struct list_head          *prev;
  struct list_head          *next;
};

# define                    LIST_HEAD_INIT(name) { &(name), &(name) }

# define                    LIST_HEAD(name) \
  struct list_head          name = LIST_HEAD_INIT(name)

# define                    INIT_LIST_HEAD(ptr) do { \
  (ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

# define                    list_entry(ptr, type, member) \
  ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

# define                    list_for_each(pos, head) \
  for (pos = (head)->next; pos != (head); \
          pos = pos->next)

# define                    list_for_each_prev(pos, head) \
  for (pos = (head)->prev; pos != (head); \
    pos = pos->prev)

# define                    list_for_each_safe(pos, n, head) \
  for (pos = (head)->next, n = pos->next; pos != (head); \
    pos = n, n = pos->next)

# define                    list_for_each_entry(pos, head, member)        \
  for (pos = list_entry((head)->next, typeof(*pos), member);  \
    &pos->member != (head);          \
    pos = list_entry(pos->member.next, typeof(*pos), member))

# define                    list_for_each_entry_safe(pos, n, head, member)      \
  for (pos = list_entry((head)->next, typeof(*pos), member),  \
    n = list_entry(pos->member.next, typeof(*pos), member); \
      &pos->member != (head);          \
      pos = n, n = list_entry(n->member.next, typeof(*n), member))


static inline void        __list_add(struct list_head *new, struct list_head *prev, struct list_head *next);
static inline void        list_add(struct list_head *new, struct list_head *head);
static inline void        list_add_tail(struct list_head *new, struct list_head *head);
static inline void        __list_del(struct list_head *prev, struct list_head *next);
static inline void        list_del(struct list_head *entry);
static inline void        list_del_init(struct list_head *entry);
static inline void        list_move(struct list_head *list, struct list_head *head);
static inline void        list_move_tail(struct list_head *list, struct list_head *head);
static inline int         list_empty(struct list_head *head);
static inline void        __list_splice(struct list_head *list, struct list_head *head);
static inline void        list_splice(struct list_head *list, struct list_head *head);
static inline void        list_splice_init(struct list_head *list, struct list_head *head);


#endif                      /*__LIST_H__*/

