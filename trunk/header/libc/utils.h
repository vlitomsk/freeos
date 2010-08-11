#ifndef UTILS_H
#define UTILS_H

extern void *memcpy(void *dest, const void *src, int count);
extern void *memset(void *dest, char val, int count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
extern int strlen(const char *str);
extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);
extern unsigned long inportl(unsigned short port);
extern void outportl(unsigned short _port, unsigned long _data);
extern unsigned short inportw(unsigned short _port);
extern void outportw(unsigned short _port, unsigned short _data);

#endif
		
