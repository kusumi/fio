#ifndef FIO_OS_DRAGONFLY_H
#define FIO_OS_DRAGONFLY_H

#define	FIO_OS	os_dragonfly

//#define _KERNEL_STRUCTURES
#include <errno.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/usched.h>

#include "../file.h"

#define FIO_HAVE_CPU_AFFINITY
#define FIO_HAVE_ODIRECT
#define FIO_USE_GENERIC_BDEV_SIZE
#define FIO_USE_GENERIC_RAND
#define FIO_USE_GENERIC_INIT_RANDOM_STATE
#define FIO_HAVE_GETTID

#define OS_MAP_ANON		MAP_ANON

typedef struct {
	uint64_t ary[4];
} os_cpu_mask_t;

#define CPU_MASK_SIMPLE(cpu)		((uint64_t)1 << (cpu))

#define CPU_MASK_ASSZERO(mask)		do {			\
					(mask)->ary[0] = 0;	\
					(mask)->ary[1] = 0;	\
					(mask)->ary[2] = 0;	\
					(mask)->ary[3] = 0;	\
					} while(0)

#define CPU_MASK_ORBIT(mask, i)		((mask)->ary[((i) >> 6) & 3] |= \
					CPU_MASK_SIMPLE((i) & 63))

#define CPU_MASK_TESTBIT(val, i)	((val)->ary[((i) >> 6) & 3] & \
					CPU_MASK_SIMPLE((i) & 63))

#define fio_cpu_clear(mask, cpu)	CPU_MASK_ASSZERO((mask))
#define fio_cpu_set(mask, cpu)		CPU_MASK_ORBIT((mask), (cpu))
#define fio_cpu_isset(mask, cpu)	CPU_MASK_TESTBIT((mask), (cpu))
#define fio_cpu_count(mask)		(0)

#define CPU_COUNT 1

static inline int fio_cpuset_init(os_cpu_mask_t *mask)
{
	CPU_MASK_ASSZERO(mask);
        return 0;
}

static inline int fio_cpuset_exit(os_cpu_mask_t *mask)
{
        return 0;
}

static inline void fio_getaffinity(int pid, os_cpu_mask_t *mask)
{
}

static inline int fio_setaffinity(int pid, os_cpu_mask_t mask)
{
	return 0;
}

#define FIO_MAX_CPUS			MAXCPU

#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN 4096
#endif

#define fio_swap16(x)	bswap16(x)
#define fio_swap32(x)	bswap32(x)
#define fio_swap64(x)	bswap64(x)

typedef off_t off64_t;

static inline int blockdev_invalidate_cache(struct fio_file *f)
{
	return EINVAL;
}

static inline unsigned long long os_phys_mem(void)
{
	int mib[2] = { CTL_HW, HW_PHYSMEM };
	uint64_t mem;
	size_t len = sizeof(mem);

	sysctl(mib, 2, &mem, &len, NULL, 0);
	return mem;
}

static inline int gettid(void)
{
	return (int) lwp_gettid();
}

#ifdef MADV_FREE
#define FIO_MADV_FREE	MADV_FREE
#endif

#endif
