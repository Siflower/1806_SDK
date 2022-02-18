#ifndef UART_H
#define UART_H
#include <sf_mmap.h>
#include <types.h>

#define UART_BASE_INTERVAL       (SIFLOWER_UART_OFFSET)
#define UART0_BASE_ADDR          (SIFLOWER_UART_BASE)
#define UART1_BASE_ADDR          (UART0_BASE_ADDR + UART_BASE_INTERVAL)
#define UART2_BASE_ADDR          (UART1_BASE_ADDR + UART_BASE_INTERVAL)
#define UART3_BASE_ADDR          (UART2_BASE_ADDR + UART_BASE_INTERVAL)


#define UART3_DR                 (UART3_BASE_ADDR + 0x00)  // Data Register
#define UART3_RSR                (UART3_BASE_ADDR + 0x04)  // Receive Status Register
#define UART3_ECR                (UART3_BASE_ADDR + 0x04)  // Error Clear Register
#define UART3_FR                 (UART3_BASE_ADDR + 0x18)  // Flag Register
#define UART3_ILPR               (UART3_BASE_ADDR + 0x20)  // IrDA Low-Power Counter Register
#define UART3_IBRD               (UART3_BASE_ADDR + 0x24)  // Integer Baud Rate Register
#define UART3_FBRD               (UART3_BASE_ADDR + 0x28)  // Fractional Baud Rate Register
#define UART3_LCRH               (UART3_BASE_ADDR + 0x2c)  // Line Control Register
#define UART3_CR                 (UART3_BASE_ADDR + 0x30)  // Control Register
#define UART3_IFLS               (UART3_BASE_ADDR + 0x34)  // Interrupt FIFO Level Select Register
#define UART3_IMSC               (UART3_BASE_ADDR + 0x38)  // Interrupt Mask Set/Clear Register
#define UART3_RIS                (UART3_BASE_ADDR + 0x3c)  // Raw Interrupt Status Register
#define UART3_MIS                (UART3_BASE_ADDR + 0x40)  // Masked Interrupt Status Register
#define UART3_ICR                (UART3_BASE_ADDR + 0x44)  // Interrupt Clear Register
#define UART3_DMACR              (UART3_BASE_ADDR + 0x48)  // DMA Control Register

#define UART2_DR                 (UART2_BASE_ADDR + 0x00)  // Data Register
#define UART2_RSR                (UART2_BASE_ADDR + 0x04)  // Receive Status Register
#define UART2_ECR                (UART2_BASE_ADDR + 0x04)  // Error Clear Register
#define UART2_FR                 (UART2_BASE_ADDR + 0x18)  // Flag Register
#define UART2_ILPR               (UART2_BASE_ADDR + 0x20)  // IrDA Low-Power Counter Register
#define UART2_IBRD               (UART2_BASE_ADDR + 0x24)  // Integer Baud Rate Register
#define UART2_FBRD               (UART2_BASE_ADDR + 0x28)  // Fractional Baud Rate Register
#define UART2_LCRH               (UART2_BASE_ADDR + 0x2c)  // Line Control Register
#define UART2_CR                 (UART2_BASE_ADDR + 0x30)  // Control Register
#define UART2_IFLS               (UART2_BASE_ADDR + 0x34)  // Interrupt FIFO Level Select Register
#define UART2_IMSC               (UART2_BASE_ADDR + 0x38)  // Interrupt Mask Set/Clear Register
#define UART2_RIS                (UART2_BASE_ADDR + 0x3c)  // Raw Interrupt Status Register
#define UART2_MIS                (UART2_BASE_ADDR + 0x40)  // Masked Interrupt Status Register
#define UART2_ICR                (UART2_BASE_ADDR + 0x44)  // Interrupt Clear Register
#define UART2_DMACR              (UART2_BASE_ADDR + 0x48)  // DMA Control Register

#define UART1_DR                 (UART1_BASE_ADDR + 0x00)  // Data Register
#define UART1_RSR                (UART1_BASE_ADDR + 0x04)  // Receive Status Register
#define UART1_ECR                (UART1_BASE_ADDR + 0x04)  // Error Clear Register
#define UART1_FR                 (UART1_BASE_ADDR + 0x18)  // Flag Register
#define UART1_ILPR               (UART1_BASE_ADDR + 0x20)  // IrDA Low-Power Counter Register
#define UART1_IBRD               (UART1_BASE_ADDR + 0x24)  // Integer Baud Rate Register
#define UART1_FBRD               (UART1_BASE_ADDR + 0x28)  // Fractional Baud Rate Register
#define UART1_LCRH               (UART1_BASE_ADDR + 0x2c)  // Line Control Register
#define UART1_CR                 (UART1_BASE_ADDR + 0x30)  // Control Register
#define UART1_IFLS               (UART1_BASE_ADDR + 0x34)  // Interrupt FIFO Level Select Register
#define UART1_IMSC               (UART1_BASE_ADDR + 0x38)  // Interrupt Mask Set/Clear Register
#define UART1_RIS                (UART1_BASE_ADDR + 0x3c)  // Raw Interrupt Status Register
#define UART1_MIS                (UART1_BASE_ADDR + 0x40)  // Masked Interrupt Status Register
#define UART1_ICR                (UART1_BASE_ADDR + 0x44)  // Interrupt Clear Register
#define UART1_DMACR              (UART1_BASE_ADDR + 0x48)  // DMA Control Register

#define UART0_DR                 (UART0_BASE_ADDR + 0x00)  // Data Register
#define UART0_RSR                (UART0_BASE_ADDR + 0x04)  // Receive Status Register
#define UART0_ECR                (UART0_BASE_ADDR + 0x04)  // Error Clear Register
#define UART0_FR                 (UART0_BASE_ADDR + 0x18)  // Flag Register
#define UART0_ILPR               (UART0_BASE_ADDR + 0x20)  // IrDA Low-Power Counter Register
#define UART0_IBRD               (UART0_BASE_ADDR + 0x24)  // Integer Baud Rate Register
#define UART0_FBRD               (UART0_BASE_ADDR + 0x28)  // Fractional Baud Rate Register
#define UART0_LCRH               (UART0_BASE_ADDR + 0x2c)  // Line Control Register
#define UART0_CR                 (UART0_BASE_ADDR + 0x30)  // Control Register
#define UART0_IFLS               (UART0_BASE_ADDR + 0x34)  // Interrupt FIFO Level Select Register
#define UART0_IMSC               (UART0_BASE_ADDR + 0x38)  // Interrupt Mask Set/Clear Register
#define UART0_RIS                (UART0_BASE_ADDR + 0x3c)  // Raw Interrupt Status Register
#define UART0_MIS                (UART0_BASE_ADDR + 0x40)  // Masked Interrupt Status Register
#define UART0_ICR                (UART0_BASE_ADDR + 0x44)  // Interrupt Clear Register
#define UART0_DMACR              (UART0_BASE_ADDR + 0x48)  // DMA Control Register

#define UART_FR_CTS         (1<<0)
#define UART_FR_RXFE		(1<<4)
#define UART_FR_TXFF		(1<<5)
#define UART_FR_TXFE		(1<<7)
#define UART_LCRH_BRK		(1<<0)
#define UART_LCRH_PEN		(1<<1)
#define UART_LCRH_EPS		(1<<2)
#define UART_LCRH_STP2		(1<<3)
#define UART_LCRH_FEN		(1<<4)
#define UART_LCRH_WLEN0		(1<<5)
#define UART_LCRH_WLEN1		(1<<6)
#define UART_LCRH_SPS		(1<<7)
#define UART_CR_RTS         (1<<11)

#define UART_SRR	        (UART_SYSM_ADDR + 0x0)		/*need to change --just a supposition*/
#define UART_CGER	        (UART_SYSM_ADDR + 0x4)
#define UART_CLK			(SF_UART_CLK)
#define UART_FIFO_DEPTH     32
#define UART_DEBUG_CHANNEL  0

extern void uart_tx_debug_msg(char * buf,unsigned int len);
extern void uart_rx_debug_msg(char * buf,unsigned int len);
extern int uart_init_debug_channel(int channel);
extern void uart_rx_msg(u8 channel,char * buf,unsigned int len);
extern char uart_getc(u32 channel);
extern char uart_getc_mdelay(u32 channel, u32 mdelay);

#endif          /*UART_H*/
