#include <main.h>

#include<screen.h>
#include<gdt.h>
#include<idt.h>
#include<isr.h>
#include<irq.h>
#include<timer.h>
#include<keyboard.h>
#include<fat32.h>

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
