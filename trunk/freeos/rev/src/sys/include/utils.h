#ifndef UTILS_H
#define UTILS_H

extern void delay(int a);

extern void outb(unsigned short port, unsigned char value);
extern void outw(unsigned short port, unsigned short value);
extern void outl(unsigned short port, unsigned long value);

extern unsigned char inb(unsigned short port);
extern unsigned short inw(unsigned short port);
extern unsigned long inl(unsigned short port);

#endif
