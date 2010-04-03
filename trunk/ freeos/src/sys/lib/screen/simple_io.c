#include <config/screen.h>

//Тов.Леонид, "атрибут" пишется всего с двумя "т",вместо 3 :) 

int tty_cursor;    //положение курсора
int tty_attribute; //текущий атрибут символа
char* video= VIDEO_RAM; // указатель на видеобуфер

//Инициализация tty
void init_tty() {
  tty_cursor = 0;
  tty_attribute = 7;
}

<<<<<<< .mine
//Смена текущего атрибута символа
void textcolor(unsigned int c) {
=======
//Смена текущего атрибута символа
void textcolor(unsigned char c) {
>>>>>>> .r7
  tty_attribute = c;
}

<<<<<<< .mine
// like pascal :)
extern int gotoxy(unsigned int x, unsigned int y) {
	if (x*y > VIDEO_HEIGHT*VIDEO_WIDTH) return 1;
	if ((x >= VIDEO_WIDTH) || (y >= VIDEO_HEIGHT)) return 1;
	tty_cursor = y * VIDEO_WIDTH - VIDEO_WIDTH + x - 1;

	return 0;	
=======
// like pascal :)
extern int gotoxy(unsigned int x, unsigned int y) {
	if (x*y > VIDEO_HEIGHT*VIDEO_WIDTH) return 1;
	if ((x >= VIDEO_WIDTH) || (y >= VIDEO_HEIGHT)) return 1;
	tty_cursor = y * VIDEO_WIDTH / 10 - VIDEO_WIDTH / 10 + x - 1;

	return 0;	
}

//Очистка экрана
void clear(void) { 
	int i;
  for (i ^= i; i < VIDEO_HEIGHT*VIDEO_WIDTH; ++i)
    *(video + i*2) = ' ';    
  
  tty_cursor ^= tty_cursor;
>>>>>>> .r7
}

<<<<<<< .mine
//Очистка экрана
void clear(unsigned int c) { 
	int i;
	int a=0;
	for (i ^= i; i < VIDEO_HEIGHT*VIDEO_WIDTH; ++i)
	{
		*(video + i*2) = ' ';    
		*(video + 2*(a++)+1) = c ;
	}
  tty_cursor ^= tty_cursor;
}

=======
>>>>>>> .r7
<<<<<<< .mine
//Вывод одного символа в режиме телетапа
void putchar(char c) {  
=======
//Вывод одного символа в режиме телетайпа
void putchar(char c) {  
>>>>>>> .r7
  int i;
  switch (c) {

  	case '\n': //Если это символ новой строки
  					  tty_cursor=tty_cursor+VIDEO_WIDTH-tty_cursor%VIDEO_WIDTH;  					  
    				break;
    
	  default:
  					  *(video + tty_cursor*2) = c;
					    *(video + 2*(tty_cursor++)+1) = tty_attribute;					    
				    break;
  }

  //Если курсор вышел за границу экрана, сдвинем экран вверх на одну строку
  if(tty_cursor>VIDEO_WIDTH*VIDEO_HEIGHT) {
    for(i = VIDEO_WIDTH*2; i <= VIDEO_WIDTH*VIDEO_HEIGHT*2+VIDEO_WIDTH*2; ++i)
      *(video+i-VIDEO_WIDTH*2)=*(video+i);
    
    tty_cursor-=VIDEO_WIDTH;
  }
}

//Вывод строки, заканчивающейся нуль-символом
void puts(const char *s)
{
  while(*s) {
    putchar(*s);
    s++;
  }
}

