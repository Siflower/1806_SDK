#ifndef IRQ_HANDLE_H
#define IRQ_HANDLE_H

#define HARD_IRQ_PIN_NUM 6
#define GIC_EIC_INTERRUPT_NUM 256
#define GIC_HIGH_LV 1
#define GIC_LOW_LV 2
#define GIC_RISING_EDGE 3
#define GIC_FALLING_EDGE 4

#define GIC_TRIG_EDGE	1
#define GIC_SH_WEDGE 0x0280

typedef int (*irq_handle_type)(int irqnum);
extern int Register_interrupt_num(int irqnum,irq_handle_type handle,int trigger);
int irq_common_handler(void);
extern int gic_set_trig(int irqnum,int trig);
extern int gic_enable_irq(int irqnum);
extern int gic_disable_irq(int irqnum);
extern int cpu_enable_irq();
extern int cpu_disable_irq();
extern int unregister_irq_num(int irqnum);

#endif
