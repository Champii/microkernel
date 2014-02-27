.global linux_exit
.global linux_write
.global linux_mmap
.global linux_munmap
.global linux_mq_open
.global linux_mq_timedsend
.global linux_mq_timedreceive
.global linux_mq_unlink

#if __x86_64__

#define LINUX_SYSCALL_EXIT            60
#define LINUX_SYSCALL_WRITE           1
#define LINUX_SYSCALL_MMAP            9
#define LINUX_SYSCALL_MUNMAP          11
#define LINUX_SYSCALL_MQ_OPEN         240
#define LINUX_SYSCALL_MQ_UNLINK       241
#define LINUX_SYSCALL_MQ_TIMEDSEND    242
#define LINUX_SYSCALL_MQ_TIMEDRECEIVE 243

linux_exit:
  mov $LINUX_SYSCALL_EXIT, %eax
  syscall
  ret

linux_write:
  mov $LINUX_SYSCALL_WRITE, %eax
  syscall
  ret

linux_mmap:
  mov %rcx, %r10
  mov $LINUX_SYSCALL_MMAP, %eax
  syscall
  ret

linux_munmap:
  mov $LINUX_SYSCALL_MUNMAP, %eax
  syscall
  ret

linux_mq_open:
  mov %rcx, %r10
  mov $LINUX_SYSCALL_MQ_OPEN, %eax
  syscall
  ret

linux_mq_unlink:
  mov $LINUX_SYSCALL_MQ_UNLINK, %eax
  syscall
  ret

linux_mq_timedsend:
  mov %rcx, %r10
  mov $LINUX_SYSCALL_MQ_TIMEDSEND, %eax
  syscall
  ret

linux_mq_timedreceive:
  mov %rcx, %r10
  mov $LINUX_SYSCALL_MQ_TIMEDRECEIVE, %eax
  syscall
  ret

#else

#define LINUX_SYSCALL_EXIT            1
#define LINUX_SYSCALL_WRITE           4
#define LINUX_SYSCALL_MMAP            192
#define LINUX_SYSCALL_MUNMAP          91
#define LINUX_SYSCALL_MQ_OPEN         277
#define LINUX_SYSCALL_MQ_UNLINK       278
#define LINUX_SYSCALL_MQ_TIMEDSEND    279
#define LINUX_SYSCALL_MQ_TIMEDRECEIVE 280

linux_exit:
  pushl %ebx

  mov 8(%esp), %ebx
  mov $LINUX_SYSCALL_EXIT, %eax
  int $0x80

  popl %ebx
  ret

linux_write:
  pushl %ebx
  pushl %ecx
  pushl %edx

  mov 16(%esp), %ebx
  mov 20(%esp), %ecx
  mov 24(%esp), %ebx
  mov $LINUX_SYSCALL_WRITE, %eax
  int $0x80

  popl %edx
  popl %ebx
  popl %ecx
  ret

linux_mmap:
  pushl %ebx
  pushl %ecx
  pushl %edx
  pushl %esi
  pushl %edi
  pushl %ebp

  mov 28(%esp), %ebx
  mov 32(%esp), %ecx
  mov 36(%esp), %edx
  mov 40(%esp), %esi
  mov 44(%esp), %edi
  mov 48(%esp), %ebp
  mov $LINUX_SYSCALL_MMAP, %eax
  int $0x80

  popl %ebp
  popl %edi
  popl %esi
  popl %edx
  popl %ebx
  popl %ecx
  ret

linux_munmap:
  pushl %ebx
  pushl %ecx

  mov 12(%esp), %ebx
  mov 16(%esp), %ecx
  mov $LINUX_SYSCALL_MUNMAP, %eax
  int $0x80

  popl %ebx
  popl %ecx
  ret

linux_mq_open:
  pushl %ebx
  pushl %ecx
  pushl %edx
  pushl %esi

  mov 20(%esp), %ebx
  mov 24(%esp), %ecx
  mov 28(%esp), %edx
  mov 32(%esp), %esi
  mov $LINUX_SYSCALL_MQ_OPEN, %eax
  int $0x80

  popl %esi
  popl %edx
  popl %ebx
  popl %ecx
  ret

linux_mq_unlink:
  pushl %ebx

  mov 8(%esp), %ebx
  mov $LINUX_SYSCALL_MQ_UNLINK, %eax
  int $0x80

  popl %ebx
  ret

linux_mq_timedsend:
  pushl %ebx
  pushl %ecx
  pushl %edx
  pushl %esi
  pushl %edi

  mov 24(%esp), %ebx
  mov 28(%esp), %ecx
  mov 32(%esp), %edx
  mov 36(%esp), %esi
  mov 40(%esp), %edi
  mov $LINUX_SYSCALL_MQ_TIMEDSEND, %eax
  int $0x80

  popl %edi
  popl %esi
  popl %edx
  popl %ebx
  popl %ecx
  ret

linux_mq_timedreceive:
  pushl %ebx
  pushl %ecx
  pushl %edx
  pushl %esi
  pushl %edi

  mov 24(%esp), %ebx
  mov 28(%esp), %ecx
  mov 32(%esp), %edx
  mov 36(%esp), %esi
  mov 40(%esp), %edi
  mov $LINUX_SYSCALL_MQ_TIMEDRECEIVE, %eax
  int $0x80

  popl %edi
  popl %esi
  popl %edx
  popl %ebx
  popl %ecx
  ret

#endif
