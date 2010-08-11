#include<device/io/screen.h>
#include<tables/gdt.h>
#include<tables/idt.h>
#include<tables/isr.h>
#include<tables/irq.h>
#include<device/timer.h>
#include<device/io/keyboard.h>
#include<device/hd/fat32.h>

void kmain() {
	
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();	
	
	timer_install();

	keyboard_install();  
	__asm__ __volatile__ ("sti");
	
	init_video();
 	puts("hello world gdt idt isr irq timer keyboard  [OK]\n");
	
/*	while(1==1){
	}*/
}
