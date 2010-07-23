BIN=bin/cdrom/root/
SRC=src/
LIB=lib/

CFLAGS	= -m32 -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -fno-stack-protector -c -I./header/ -I./header/libc 
LFLAGS	= -m elf_i386 -T linker.ld

CC		= gcc $(CFLAGS) -o
NA		= nasm -f aout  -o
LD		= ld  $(LFLAGS) -o 


all: link

start:
	$(NA) $(LIB)multiboot.o $(SRC)multiboot.S

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
mmanager:
	$(CC) $(LIB)mman.o $(SRC)memory_manager.c
dma:
	$(CC) $(LIB)dma.o $(SRC)dma.c
#pci:
#	$(CC) $(LIB)pci.o $(SRC)pci.c
#ata:
#	$(CC) $(LIB)ata.o $(SRC)ata.c
libc:
	$(CC) $(LIB)stdlib.o $(SRC)libc/stdlib.c
clean:
	rm $(LIB)*.o -f
	rm $(LIB)libc/*.o -f

link: clean start main utils screen gdt idt isr irq timer keys mmanager libc dma
	$(LD) $(BIN)kernel.bin $(LIB)*.o
	
