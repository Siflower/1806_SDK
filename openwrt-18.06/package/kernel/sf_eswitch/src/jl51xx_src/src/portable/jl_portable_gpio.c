#include "jl_types.h"
#include "jl_error.h"

#include "portable/jl_portable.h"

jl_ret_t port_gpio_request(struct gpio_descriptor_s *gpio)
{
	(void) gpio;

	return JL_ERR_OK;
}

jl_ret_t port_gpio_free(struct gpio_descriptor_s *gpio)
{
	(void) gpio;

	return JL_ERR_OK;
}

/*dir: 0: input, 1: output*/
void port_gpio_set_dir(struct gpio_descriptor_s *gpio, jl_int32 dir)
{
	(void) gpio;
	(void) dir;
}

void port_gpio_set_value(struct gpio_descriptor_s *gpio, jl_int32 val)
{
	(void) gpio;
	(void) val;
}

jl_int32 port_gpio_get_value(struct gpio_descriptor_s *gpio)
{
	(void) gpio;

	return JL_ERR_OK;
}
