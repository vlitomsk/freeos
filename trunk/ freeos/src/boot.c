
#include <multiboot/multiboot.h>
#include <screen/simple_io.h>

void kernel_main()
{
	init_tty();
	clear(0xF7);
	int i = 0;
	i=10;
	while(i<90){
		textcolor(0xF0);
		gotoxy(i,20);
		puts("-");
		delay(100);
		i++;
		if (i==71)	{ i=10; clear(0xF7);}
	}
}

