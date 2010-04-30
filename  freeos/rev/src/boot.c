
#include <multiboot/multiboot.h>
#include <screen/screen.h>

void kernel_main()
{
    __SCREEN s;
    char *sym[2] = {
        " freeos:) ", "helloworld"
    };
    int c1[10] = {0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F};
    int c2[10] = {0x1F,0x2F,0x3F,0x4F,0x5F,0x6F,0x7F,0x8F,0x9F,0xAF};
    int *color[2];
    color[0] = c1;
    color[1] = c2;
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
		    put_array(31,15,10,2,sym,color);
	    }
	}
}

