#ifndef IRQ_H
#define IRQ_H

#include<utils.h>
#include<idt.h>
#include<isr.h>

extern void irq_install_handler(int irq, void (*handler)(struct regs *r));
extern void irq_uninstall_handler(int irq);
extern void irq_remap(void);
extern void irq_install();
extern void irq_handler(struct regs *r);
#endif
