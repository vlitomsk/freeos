
#include <multiboot/multiboot.h>
#include <screen/simple_io.h>

void kernel_main()
{
	init_tty();
	clear();

	puts("It's work)01010011\n");
}

