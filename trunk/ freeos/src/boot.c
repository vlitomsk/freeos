
#include <multiboot/multiboot.h>
#include <screen/screen.h>

void kernel_main()
{
    __SCREEN s;
    setscreen(&s);
	init_tty();
	clear(0xF7);
	gotoxy(31,15);
	textcolor(0x13);
	put_str("FreeOS Hello World!");
	int i = 0;
	i=10;
	while(i<90){
		textcolor(0xF0);
		gotoxy(i,20);
		put_str("-");
		delay(100);
		i++;
		if (i==71)	{ 
		    i=10; 
		    clear(0xF7); 
		    gotoxy(31,15);
	        textcolor(0x13);
	        put_str("FreeOS Hello World!");
	    }
	}
}

