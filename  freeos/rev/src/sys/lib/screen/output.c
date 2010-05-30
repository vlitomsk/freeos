#include <screen/screen.h>
/*#include <config/video.h>*/

//static char* video = (char *)VIDEO_RAM;

//Вывод одного символа в режиме телетапа
void put_char(char c) {  
    int i;
    switch (c) {
    case '\n': //Если это символ новой строки
        screens[numscr]->tty_cursor=screens[numscr]->tty_cursor+VIDEO_WIDTH-screens[numscr]->tty_cursor%VIDEO_WIDTH;  					  
        break;
    default:
        *(video + screens[numscr]->tty_cursor*2) = c;
        *(video + 2*(screens[numscr]->tty_cursor++)+1) = screens[numscr]->tty_attribute;
        *(screens[numscr]->scr + screens[numscr]->tty_cursor*2) = c;
        *(screens[numscr]->scr + 2*(screens[numscr]->tty_cursor)+1) = screens[numscr]->tty_attribute;					    					    
        break;
    }
    
    //Если курсор вышел за границу экрана, сдвинем экран вверх на одну строку
    if(screens[numscr]->tty_cursor>VIDEO_WIDTH*VIDEO_HEIGHT) {
        for(i = VIDEO_WIDTH*2; i <= VIDEO_WIDTH*VIDEO_HEIGHT*2+VIDEO_WIDTH*2; ++i){
            *(video+i-VIDEO_WIDTH*2)=*(video+i);
            *(screens[numscr]->scr+i-VIDEO_WIDTH*2)=*(screens[numscr]->scr+i);
        }
        screens[numscr]->tty_cursor-=VIDEO_WIDTH;
    }
}

//Вывод строки, заканчивающейся нуль-символом
void put_str(const char *s){
  while(*s) {
    put_char(*s);
    s++;
  }
}

void put_int(int a) {
	if (a == 0) {
	    put_str("0\0");
	    return;
	}
		
	//char* mystr = (char*)malloc(sizeof(char) * 100);
	char mystr[100];
	int pos = 0;
 	if (a < 0) {
		mystr[pos++] = '-';
		a = -a;
	}
											
	//int* stack = (int*)malloc(sizeof(int) * 100);
	int stack[100];
	int stacklen = 0;
	for (;a != 0;) {
		int tmp = 0;
		tmp = a % 10;
		a -= tmp;
		a /= 10;
																	
		stack[stacklen++] = tmp;	
	}
																																	
	for (a = stacklen-1; a >= 0; a--) 
			mystr[pos++] = (char)(stack[a]+48); 

	//	free(stack);
																																					
	mystr[pos] = '\0';

	put_str(mystr);
}

//вывод массива символов sym  с соответствующим цветом символа и фона из color
int put_array(int x, int y, int lx, int ly, char *sym[], int *color[]){
    if(x + lx > VIDEO_WIDTH) return 0;
    int i,j;
    for(i = 0; i < lx; ++i)
        for(j = 0; j < ly; ++j){
            gotoxy(i+x,j+y);
            textcolor(color[j][i]);
            put_char(sym[j][i]);
        }
    return 1;
}
/*
Он носил красную гимнастерку,
Синий зеленый капюшон,
Он посмотрел очень хорошее.
У него большая авантюра
Среди травы
Свежий воздух, наконец.
Бары, рестораны, выжидал.
И тогда один день - ура!
Еще один способ для гномов сказать
Урааааааааааа!
*/
