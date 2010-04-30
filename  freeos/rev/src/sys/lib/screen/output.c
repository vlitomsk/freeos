#include <screen/screen.h>
/*#include <config/video.h>*/

static char* video= VIDEO_RAM;

//Вывод одного символа в режиме телетапа
void putchar(char c) {  
  int i;
  switch (c) {

  	case '\n': //Если это символ новой строки
  					  defaultscreen->tty_cursor=defaultscreen->tty_cursor+VIDEO_WIDTH-defaultscreen->tty_cursor%VIDEO_WIDTH;  					  
    				break;
    
	  default:
  					  *(video + defaultscreen->tty_cursor*2) = c;
					    *(video + 2*(defaultscreen->tty_cursor++)+1) = defaultscreen->tty_attribute;					    
				    break;
  }

  //Если курсор вышел за границу экрана, сдвинем экран вверх на одну строку
  if(defaultscreen->tty_cursor>VIDEO_WIDTH*VIDEO_HEIGHT) {
    for(i = VIDEO_WIDTH*2; i <= VIDEO_WIDTH*VIDEO_HEIGHT*2+VIDEO_WIDTH*2; ++i)
      *(video+i-VIDEO_WIDTH*2)=*(video+i);
    
    defaultscreen->tty_cursor-=VIDEO_WIDTH;
  }
}

//Вывод строки, заканчивающейся нуль-символом
void put_str(const char *s){
  while(*s) {
    putchar(*s);
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
            putchar(sym[j][i]);
        }
    return 1;
}
