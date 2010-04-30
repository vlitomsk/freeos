#	03.04.2010 by Sv9toi
#	Makefile for all 
#	в планах:
#		разбиение на блоки
#		реорганизация
LIB		= ../lib
SRC		= ./src
BIN		= ../build/cdrom/root/kernel
LARG	= $(BIN) $(LIB)/boot.o $(LIB)/mboot.o $(LIB)/vlib1.o $(LIB)/vlib2.o $(LIB)/utils.o $(LIB)/vmm_manager.o $(LIB)/paging.o $(LIB)/shared/*.o

SINC	= $(SRC)/sys/include
SLIB	= $(SRC)/sys/lib

SLINC	= $(SRC)/shared/include
SLLIB	= $(SRC)/shared/lib

CFLAGS	= -c -I$(SINC) 
LFLAGS	= -T linker.ld

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

shared:
	$(CC) $(LIB)/shared/math.o $(SLLIB)/math.c

build: boot mboot vlib utils vmm_man shared
	$(LK) $(LARG) 
