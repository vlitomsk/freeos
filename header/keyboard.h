#ifndef KEYBOARD_H
#define KEYBOARD_H

#include<idt.h>
#include<isr.h>

extern void keyboard_handler(struct regs *r);
extern void keyboard_install();

#endif
