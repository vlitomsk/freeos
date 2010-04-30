#include <utils.h>

void delay(int a) {
	float b = 0;
	while (b != a * 1000) 
		b++;	
}

void outb(unsigned short port, unsigned char value) {
	asm("outb %0,%1":: "a"(value), "d"(port));
}
void outw(unsigned short port, unsigned short value) {
	asm("outw %0,%1":: "a"(value), "d"(port));
}
void outl(unsigned short port, unsigned long value) {
	asm("outl %0,%1":: "a"(value), "d"(port));
}

unsigned char inb(unsigned short port) {
	unsigned char value;
	asm("inb %1, %0": "=a"(value): "d"(port));
	return value;
}
unsigned short inw(unsigned short port) {
	unsigned short value;
	asm("inw %1, %0": "=a"(value): "d"(port));
	return value;
}
unsigned long inl(unsigned short port) {
	unsigned long value;
	asm("inl %1, %0": "=a"(value): "d"(port));
	return value;
}

