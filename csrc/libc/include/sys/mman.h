/*
 * File: sys/mman.h
 * Author: Julien Freche <julien.freche@lse.epita.fr>
 *
 * Description: Mapping related functions
 *
 */

#ifndef SYS_MMAN_H_
# define SYS_MMAN_H_

# include <sys/types.h>
# include <stddef.h>

// FIXME: move this define to another location
# define PAGE_SHIFT 12

# define MAP_ANONYMOUS 1
# define MAP_PRIVATE 2
# define MAP_FAILED ((void*)0)
# define PROT_READ 1
# define PROT_WRITE 2
# define PROT_EXEC 4

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void* mmap(void *vaddr, int rights, size_t size);
void* mmap_phys(void *vaddr, void *paddr, int rights, size_t size);
int munmap(void *vaddr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !SYS_MMAN_H_ */
