#ifndef SIMPLE_IO_H
#define SIMPLE_IO_H

extern void	init_tty();
extern void	textcolor(unsigned int c);
extern int	gotoxy(unsigned int x, unsigned int y);
extern void	clear(unsigned int c);
extern void	putchar(char c);
extern void	put_str(const char *s);
extern void	put_int(int a);

#endif
