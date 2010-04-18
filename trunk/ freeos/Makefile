#	03.04.2010 by Sv9toi
#	Makefile for all 
#	в планах:
#		разбиение на блоки
#		реорганизация
LIB		= lib
SRC		= src
BIN		= ../cdrom/root/kernel
LARG	= $(BIN) $(LIB)/boot.o $(LIB)/mboot.o $(LIB)/vlib.o $(LIB)/utils.o $(LIB)/vmm_manager.o $(LIB)/paging.o

SINC	= src/sys/include
SLIB	= src/sys/lib

CFLAGS	= -c -I$(SINC)
LFLAGS	= -m elf_i386 -Ttext 0x200000

CC		= gcc $(CFLAGS) -fno-stack-protector -o
SC		= gcc $(CFLAGS) -o
LK		= ld  $(LFLAGS) -o 
# targets :

all: build

boot:
	$(CC) $(LIB)/boot.o		$(SRC)/boot.c 
mboot:
	$(SC) $(LIB)/mboot.o	$(SLIB)/multiboot/multiboot.S
vlib: 
	$(CC) $(LIB)/vlib1.o            $(SLIB)/screen/output.c 
    $(CC) $(LIB)/vlib2.o            $(SLIB)/screen/screen_properties.c 
    
utils: 
	$(CC) $(LIB)/utils.o	$(SLIB)/utils.c

vmm_man:
	$(CC) $(LIB)/vmm_manager.o $(SLIB)/vmm_manager/memory_manager.c
	$(CC) $(LIB)/paging.o $(SLIB)/vmm_manager/paging.c

build: boot mboot vlib utils vmm_man

	$(LK) $(LARG)

clean:
	rm $(LIB)/* -rf
	rm $(BIN)/* -rf
