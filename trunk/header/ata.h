/* На самом деле PCI IDE c использованием дма */
#ifndef ATA_H_INCLUDED
#define ATA_H_INCLUDED

#include <stdlib.h>
#include <pci.h> // пригодилось :)
#include <utils.h>

#define ATA_DEVMASTER 0x0
#define ATA_DEVSLAVE  0x1

#define PCIIDE_CLASSCODE 0x1

struct pci_device atadev;

extern int init_device(void);

#define NONPCI_BASEADDR_M 0x1F0
#define NONPCI_BASEADDR_S 0x170

// на самом деле смещения 
#define PCIIDE_INT_REG   0x206
#define PCIIDE_STATE_REG 0x7

#define BIT_BSY  0x80
#define BIT_DRDY 0x40
#define BIT_DRQ  0x8
#define BIT_ERR  0x1

#define ata_cli(baseaddr) outportb(baseaddr + PCIIDE_INT_REG, 2)
#define ata_sti(baseaddr) outportb(baseaddr + PCIIDE_INT_REG, 0)

// timeout -  в тиках, которые определены в src/timer.c. судя по внутренностям timer_wait, в одной секунде 18 тиков. 
// !!Леонид, мэджик намбер детектед.!!!1!
// для следующих 4 строк baseaddr - это из NONPCI_BASEADDR_M/S (master/slave0 , просто спутал c pci'шным BAR, переделывать поздно :-)
extern int wait_bsy(int baseaddr, int timeout, int tries);
extern int wait_drdy(int baseaddr, int timeout, int tries);
extern int wait_drq(int baseaddr, int timeout, int tries);
#define PRD_FLAG_ENDLESS 0x8000
#define PRD_FLAG_NORMAL  0x0000
struct prd {
	u32 prd_addr;
	u16 prd_count;
	u16 prd_flag;
	u8* buff;
};
extern u32 mk_params(u8 device, u32 lba);

#define ATA_READ 0xC8
#define ATA_WRITE 0xCA
#define DIR_FROM_RAM 0
#define DIR_TO_RAM 8

extern int config_dma_rw(struct prd* prdt, u32 params, int baseaddr, int sectors_num, int operation, int direction);
extern int config_dma_read(struct prd* prdt, u32 params, int baseaddr, int sectors_num);
extern int config_dma_rw(struct prd* prdt, u32 params, int baseaddr, int sectors_num, int operation, int direction);

#endif
