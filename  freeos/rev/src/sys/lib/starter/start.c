#include<starter/start.h>
void out_logo()
{
	clear(0xFF);
	int i = 10;
	while(1){
		textcolor(0xF0);
		gotoxy(i,20);
		put_str("-");
		delay(100);
		i++;
		if (i==71)	{ 
		    i=10; 
		    clear(0xFF); 
	    }
	}
}
