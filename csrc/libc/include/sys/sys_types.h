/*
 * File: sys/sys_types.h
 * Author: Victor Aperce <vaperce@gmail.com>
 *
 * Description: basic system types
 *
 */

#ifndef SYS_TYPES_H_
#define SYS_TYPES_H_

typedef unsigned char u8;
typedef signed char i8;

typedef unsigned short u16;
typedef unsigned short le16;
typedef unsigned short be16;
typedef short i16;

typedef unsigned int u32;
typedef unsigned int le32;
typedef unsigned int be32;
typedef int i32;

#if __x86_64__
typedef unsigned long u64;
typedef unsigned long le64;
typedef unsigned long be64;
typedef long i64;
#else
typedef unsigned long long u64;
typedef unsigned long long le64;
typedef unsigned long long be64;
typedef long long i64;
#endif

typedef u16 uint16_t;
typedef i16 int16_t;

typedef u32 uint32_t;
typedef i32 int32_t;

typedef u64 uint64_t;
typedef i64 int64_t;

typedef unsigned int pid_t;

#endif /* !SYS_TYPES_H_ */
