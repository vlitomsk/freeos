#include <main.h>
#include <stdlib.h>
#include <screen.h>
#include <ata.h>

void kmain() {
	
	gdt_install();
  	idt_install();
	isrs_install();
	irq_install();	
	
    // раскоментить для проверки таймера	
	//timer_install();

	keyboard_install();  
	__asm__ __volatile__ ("sti");
	
	init_video();
	puts("hello world gdt idt isr irq timer keyboard  [OK]\n");
	
	//раскоментить для проверки на рабочесть прерываний	
	//int i;
	//i= 10/0;
	//puts(i);

	set_channel(0);
	set_timeout(1000); // даже в секундах

	struct ata_rw_info test_rw_info;
	test_rw_info.lba = 0; 
	test_rw_info.count = 512;
//	test_rw_info.buff = (u8*)malloc(sizeof(u8) * 512);	
/*	int i;
	for (i = 0; i < test_rw_info.count; ++i) {
		test_rw_info.buff[i] = i;
	}*/
	int ata_retval = ata_read(DEVICE_MASTER, &test_rw_info);
	puts("ata_read(..)'s return value: "); put_int(ata_retval); puts("\n");

	if (ata_retval == 0) {
		int i;
		for (i = 0; i < test_rw_info.count; ++i) {
			put_int((int)test_rw_info.buff[i]); puts(" ");
		}
	}
}
