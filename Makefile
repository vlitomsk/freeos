BIN=bin/rom/root/
SRC=src/
LIB=lib/

CFLAGS	= -m32 -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -c -I./header/
LFLAGS	= -m elf_i386 -T linker.ld

CC		= gcc $(CFLAGS) -o
NA		= nasm -f aout  -o
LD		= ld  $(LFLAGS) -o 


all: link

start:
	$(CC) $(LIB)multiboot.o $(SRC)multiboot.S

main:
	$(CC) $(LIB)main.o			$(SRC)main.c
utils:
	$(CC) $(LIB)utils.o			$(SRC)utils.c
screen:
	$(CC) $(LIB)screen.o		$(SRC)screen.c
gdt:
	$(CC) $(LIB)gdt.o				$(SRC)gdt.c
	$(NA) $(LIB)agdt.o			$(SRC)gdt.asm
idt:
	$(CC) $(LIB)idt.o				$(SRC)idt.c
	$(NA) $(LIB)aidt.o			$(SRC)idt.asm
isr: 
	$(CC) $(LIB)isr.o				$(SRC)isr.c
	$(NA) $(LIB)aisr.o			$(SRC)isr.asm
irq:
	$(CC) $(LIB)irq.o				$(SRC)irq.c
	$(NA) $(LIB)airq.o			$(SRC)irq.asm
timer:
	$(CC) $(LIB)timer.o			$(SRC)timer.c
keys:
	$(CC) $(LIB)keyboard.o	$(SRC)keyboard.c
clean:
	rm $(LIB)*.o &

link: clean start main utils screen gdt idt isr irq timer keys
	$(LD) $(BIN)kernel.bin $(LIB)*.o	
	