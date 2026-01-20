#include "jl_types.h"
#include "jl_error.h"

jl_ret_t port_mdio_init(void)
{
	return JL_ERR_OK;
}

jl_ret_t port_mdio_deinit(void)
{
	return JL_ERR_OK;
}

void port_mdio_write(jl_uint8 phy, jl_uint8 reg, jl_uint16 val)
{
	(void) phy;
	(void) reg;
	(void) val;
}

jl_uint16 port_mdio_read(jl_uint8 phy, jl_uint8 reg)
{
	(void) phy;
	(void) reg;

	return 0;
}
