#ifndef IDT_H
#define IDT_H

extern void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
extern void idt_install();


#endif

