#ifndef SIMPLE_IO_H
#define SIMPLE_IO_H

<<<<<<< .mine
extern void	init_tty();
extern void	textcolor(unsigned int c);
extern int	gotoxy(unsigned int x, unsigned int y);
extern void	clear(unsigned int c);
extern void	putchar(char c);
extern void	puts(const char *s);
=======
extern void	init_tty();
extern void	textcolor(unsigned char c);
extern int	gotoxy(unsigned int x, unsigned int y);
extern void	clear(void);
extern void	putchar(char c);
extern void	puts(const char *s);
>>>>>>> .r7

#endif
