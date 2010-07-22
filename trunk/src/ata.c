#include <ata.h>
#include <pci.h>
#include <timer.h>

int ata_initialized = 0;

int init_device(void) {
	if (pci_init_device(0, 0, 0, PCIIDE_CLASSCODE, &atadev) == 0)
		ata_initialized = 1;
	else
		ata_initialized = 0;

	return ata_initialized;
}


int wait_bsy(int baseaddr, int timeout, int tries) {
	if (!ata_initialized) return 1;
	int i;
	for (i = 0; i < tries; ++i) {
		u8 statereg = inportb(baseaddr + PCIIDE_STATE_REG);
		if ((statereg & BIT_BSY) == 0) return 0;
		timer_wait(timeout);
	}
	return 1;
}

// немного быдлокода :)
int wait_drdy(int baseaddr, int timeout, int tries) {
	if (!ata_initialized) return 1;
	int i;
	for (i = 0; i < tries; ++i) {
		u8 statereg = inportb(baseaddr + PCIIDE_STATE_REG);
		if (statereg & BIT_DRDY) return 0;
		timer_wait(timeout);
	}
	return 1;
}

int wait_drq(int baseaddr, int timeout, int tries) {
	if (!ata_initialized) return 1;
	while (true) {
		u8 statereg = inportb(baseaddr + PCIIDE_STATE_REG);
		if (statereg & BIT_DRQ) return 0;
	}

	return 1; // недостижимо, но все же :-)
}

u32 mk_params(u8 device, u32 lba) {
	return 0 | ((lba & (~0xF0000000)) | ((device & 0x1) << 28));
}

int config_dma_rw(struct prd* prdt, u32 params, int baseaddr, int sectors_num, int operation, int direction) {	
	if (!ata_initialized) return 1;
	if ((operation != ATA_READ) || (operation != ATA_WRITE)) return 1;
	direction &= 0x8;
	ata_cli(baseaddr);
	if (wait_bsy(baseaddr, 18, 30)) { ata_sti(); return 1; } // опять же мэджик намберс из-за этих самых меджик намберс в src/timer.c
	outportb(baseaddr + 6, (params >> 24) | 0x0E0); // ??
	if (wait_bsy(baseaddr, 18, 30) || (wait_drdy(baseaddr, 18, 30))) { ata_sti(); return 1; } // 

	int i;
	for (i = 0; i < 3; ++i) 
		outportb(baseaddr + i + 3, (params >> (i << 3)) & 0xFF); 

	outportb(baseaddr + 2, sectors_num & 0xFF);
	u32 pcibase = atadev.base_addr; // ????? то или не то ??
	outportb(pcibase + 4, (u32)prdt);
	outportb(pcibase, direction);
	outportb(baseaddr + 7, operation);
	outportb(pcibase, 9); // вжжж
	while (inportb(pcibase + 2) & 0x1) {}
	outportb(pcibase, 0); // шшшш
	ata_sti(baseaddr);

	return 0;
}

int config_dma_read(struct prd* prdt, u32 params, int baseaddr, int sectors_num) {
	return config_dma_rw(prdt, params, baseaddr, sectors_num, ATA_READ, DIR_TO_RAM);
}

int config_dma_write(struct prd* prdt, u32 params, int baseaddr, int sectors_num) {
	return config_dma_rw(prdt, params, baseaddr, sectors_num, ATA_WRITE, DIR_FROM_RAM);
}
