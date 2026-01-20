#include "jl_types.h"
#include "jl_error.h"
#include "jl_debug.h"

jl_ret_t port_spi_init(void)
{
	return JL_ERR_OK;
}

jl_ret_t port_spi_deinit(void)
{
	return JL_ERR_OK;
}

jl_ret_t port_spi_write(jl_uint8 *tx_buf, jl_uint8 *rx_buf, jl_uint32 size)
{
	(void) tx_buf;
	(void) rx_buf;
	(void) size;

	return JL_ERR_OK;
}

jl_ret_t port_spi_read(jl_uint8 *tx_buf, jl_uint8 *rx_buf, jl_uint32 size)
{
	(void) tx_buf;
	(void) rx_buf;
	(void) size;

	return JL_ERR_OK;
}
