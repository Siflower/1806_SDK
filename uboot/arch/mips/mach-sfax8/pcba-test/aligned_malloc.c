#include <common.h>
#include <stdlib.h>

#include <mach/pcba_test.h>
#include <timer.h>
#include <dm/uclass.h>

/*
 * CONFIG_SYS_CACHE_SIZE_AUTO is set in uboot,
 * so cache line size is read from cpu regs when used
 * and CONFIG_SYS_DCACHE_SIZE is set to zero.
 */
#define MIPS_INTERAPTIV_CACHELINE_SIZE 32

void *malloc_al(int size)
{
	int alignment = MIPS_INTERAPTIV_CACHELINE_SIZE;
	void* ptr = malloc(size + alignment);

	if (ptr)
	{
		if ((long)ptr % 4 != 0)
			return NULL;

		void *aligned = (void*)(((long)ptr + alignment) & ~(alignment - 1));
		((void**)aligned)[-1] = ptr;

		return aligned;
	}
	else
		return NULL;
}
void free_al(void *paligned)
{
	free(((void **)paligned)[-1]);
	return;
}

int sf_random(int n)
{
	struct udevice *udev;
	unsigned long long cnt;

	if (uclass_get_device(UCLASS_TIMER, 0, &udev) == 0) {
		if (timer_get_count(udev, &cnt) == 0)
			return (unsigned int)cnt%n;
	}
	printf("Failed to get timer for random(), use chipid instead.\n");
	cnt = read_chipid();

	return (unsigned int)cnt%n;
}

#define is_digit(c)				((c) >= '0' && (c) <= '9')
int sf_atoi(int *a, int a_len, char *s)
{
	int i, j = 0;

	for (i = 0; i < a_len; i++)
		a[i] = 0;

	for (i = 0; i < strlen(s); i++) {
		while (is_digit(s[i])) {
			a[j] = a[j] * 10 + s[i] - '0';
			i++;
		}
		j++;
		if (j >= a_len) {
			printf("warning: %s string too long.\n", __func__);
			break;
		}
	}
	/* The last one is set to -1 */
	a[j] = -1;

	return 0;
}
