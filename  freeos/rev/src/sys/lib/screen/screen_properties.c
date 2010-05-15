#include <screen/screen.h>
/*#include <config/video.h>*/

//Инициализация видеовывода
void init_video(){
    video = (char *)VIDEO_RAM;
    count = 0;
    int x = new_tty();
    set_tty(x);
}

//Инициализация tty
int new_tty() {
    __SCREEN *newscr = (__SCREEN *)malloc(sizeof(__SCREEN));
    newscr->tty_cursor = 0;
    newscr->tty_attribute = 7;
    int i;
    for(i = 0; i < VIDEO_HEIGHT*VIDEO_WIDTH; ++i){
        newscr->scr[i++] = 0;
        newscr->scr[i] = 7;        
    }
    screens[count] = *newscr;
    return ++count - 1;
}

//Смена текущего атрибута символа
void textcolor(unsigned int c) {
    screens[numscr].tty_attribute = c;
}

// like pascal :)
int gotoxy(unsigned int x, unsigned int y) {
	if (x*y > VIDEO_HEIGHT*VIDEO_WIDTH) return 1;
	if ((x >= VIDEO_WIDTH) || (y >= VIDEO_HEIGHT)) return 1;
	screens[numscr].tty_cursor = y * VIDEO_WIDTH - VIDEO_WIDTH + x - 1;
	return 0;	
}

//Очистка экрана
void clear(unsigned int c) {
	int i;
	for (i ^= i; i < VIDEO_HEIGHT*VIDEO_WIDTH; ++i)
	{
		*(video + i*2) = ' ';    
		*(video + 2*i+1) = c ;
		*(screens[numscr].scr + i*2) = ' ';    
		*(screens[numscr].scr + 2*i+1) = c ;
	}
    screens[numscr].tty_cursor ^= screens[numscr].tty_cursor;
}

//Установка текущего экрана
void set_tty(int n){
    numscr = n;
    repaint();
}

void repaint(){
    int i;
    for (i ^= i; i < VIDEO_HEIGHT*VIDEO_WIDTH; ++i)
	{
		*(video + i*2) = *(screens[numscr].scr + i*2);
		*(video + i*2+1) = *(screens[numscr].scr + i*2+1);
	}
}
/*Бегущее вперед время уносило наши мечты
Оставляя мириады крошечных существ,
пытающихся привязать нас к земле,
К жизни, тронутой медленным гниением. */
