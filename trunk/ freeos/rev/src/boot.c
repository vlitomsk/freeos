
#include <multiboot/multiboot.h>
#include <screen/screen.h>
#include <starter/start.h>

void kernel_main()
{
	__SCREEN s;
	setscreen(&s);
  init_tty();
  
  out_logo();
}

