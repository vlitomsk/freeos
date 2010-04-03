#	03.04.2010 by Sv9toi
#	Makefile for all 
#	в планах:
#		разбиение на блоки
#		реорганизация
LIB		= lib
SRC		= src
BIN		= bin
LARG	= ../cdrom/root/kernel $(LIB)/boot.o $(LIB)/mboot.o $(LIB)/vlib.o $(LIB)/utils.o

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
	$(CC) $(LIB)/vlib.o		$(SLIB)/screen/simple_io.c
utils: 
	$(CC) $(LIB)/utils.o	$(SLIB)/utils.c

build: boot mboot vlib utils

	$(LK) $(LARG)

clean:
	rm $(LIB)/*
	rm $(BIN)/*
