#include "jl_types.h"

void *port_mutex_init(void)
{
	return NULL;
	/* return mutex; */
}

void port_mutex_deinit(void *arg)
{
	(void) arg;
}

void port_mutex_lock(void *arg)
{
	(void) arg;
}

void port_mutex_unlock(void *arg)
{
	(void) arg;
}

void port_ndelay(jl_uint32 ns)
{
	(void) ns;
}

void port_udelay(jl_uint32 us)
{
	(void) us;
}
