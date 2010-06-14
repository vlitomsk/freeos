#include<starter/start.h>
void out_logo()
{
	int x = new_tty();
	set_tty(x);
	clear(0x2F);
	textcolor(0x70);
	gotoxy(25,15);
	put_str("this is second display ^_^");
	set_tty(0);
	clear(0xFF);
	int i = 10;
	int c = 0;
	while(1){
		textcolor(0xF0);
		gotoxy(i,20);
		put_str("-");
		delay(100);
		i++;
		if (i==71)	{ 
		    i=10;
		    set_tty(c == 1 ? --c : ++c);
		    if(c){
		        clear(0x2F);
	            gotoxy(25,15);
	            textcolor(0x70);
	            put_str("this is second display ^_^");
		    } else
		        clear(0xFF);
	    }
	}
}
