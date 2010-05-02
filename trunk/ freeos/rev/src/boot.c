#include <screen/screen.h>

#include <starter/start.h>

void main()
{
	__SCREEN s;
	setscreen(&s);
  init_tty();
  
  out_logo();
}

