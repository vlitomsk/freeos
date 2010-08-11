#include <device/hd/ata.h>
#include <libc/stdlib.h>
#include <libc/utils.h>
#include <device/io/screen.h>

#define ATA_BIT_BSY 7
#define ATA_BIT_DRDY 6
#define ATA_BIT_DF 5
#define ATA_BIT_DSC 4
#define ATA_BIT_DRQ 3
#define ATA_BIT_CORR 2
#define ATA_BIT_IDX 1
#define ATA_BIT_ERR 0

#define BASE1_CH0 0x1F0
#define BASE1_CH1 0x170
#define BASE2_CH0 0x3F6
#define BASE2_CH1 0x376

int base1 = BASE1_CH0, base2 = BASE2_CH0;

int ata_timeout = 1000;

void set_timeout(int t) {
	ata_timeout = t;
}

void set_channel(int ch) {
	if (~ch) {
		base1 = BASE1_CH0;
		base2 = BASE2_CH0;
		return;
	}	

	base1 = BASE1_CH1;
	base2 = BASE2_CH1;	
}

#define ata_cli() outportb(base2, 2)
#define ata_sti() outportb(base2, 0)

int wait_bit(int port, int bit_n, int onoff) {
	int i;
	int tmpmask = 1 << bit_n;
	for (i = 0; i < ata_timeout; ++i) {
		u8 tmp = inportb(port);
		if (onoff) {
			if ((tmp & tmpmask) >> bit_n) return 1;
		} else {
			if ((~(tmp & tmpmask)) >> bit_n) return 1;
		}
	}

	return 0;
}

int wait_bsy() {
	return wait_bit(base1 + 7, ATA_BIT_BSY, 0);
}

int wait_drdy() {
	return wait_bit(base1 + 7, ATA_BIT_DRDY, 1);
}

int wait_drq() {
	return wait_bit(base1 + 7, ATA_BIT_DRQ, 1);
}

int check_error() {
	return inportb(base1 + 7) & (1 << ATA_BIT_ERR);
}

#define ATA_CMD_READ 0x20
#define ATA_CMD_WRITE 0x30

#define AM_LBA 0x40

int ata_rwsect_action(int device, int cmd, int lba, u8* buffer) {
	if ((cmd != ATA_CMD_READ) && (cmd != ATA_CMD_WRITE)) return 1;
	if ((device != DEVICE_MASTER) && (device != DEVICE_SLAVE)) return 1;
	if (!wait_bsy())  return 1; 
	u8 tmp = 0xA0 | AM_LBA | device | ((lba & 0xF000000) >> 24);
	outportb(base1 + 6, tmp);

	if (!((wait_bsy()) && (wait_drdy())))  return 1; 

	outportb(base1 + 2, 1);

	outportb(base1 + 3, (lba & 0xFF));
	outportb(base1 + 4, ((lba & 0xFF00) >> 8));
	outportb(base1 + 5, ((lba & 0xFF0000) >> 16));

	outportb(base1 + 7, cmd);
	
	if (!wait_bsy()) return 1;
	if (check_error()) return 1;
	if (!wait_drq()) return 1;

	int i;
	for (i = 0; i < 256; i++) {
		if (cmd == ATA_CMD_READ) {		
			u16 temp = inportw(base1);
			buffer[i * 2 + 1] = (u8)((temp & 0xFF));
			buffer[i * 2] = (u8)((temp & 0xFF00) >> 8);
//			put_int(buffer[i * 2]); puts(" ");put_int(buffer[i * 2 + 1]);puts(" ");
//			put_int(temp); puts(" ");
		}
		if (cmd == ATA_CMD_WRITE) { 
			outportw(base1, ((u16)(buffer[i * 2]) << 8) | ((u16)(buffer[i * 2 + 1])));
//			put_int(((u16)(buffer[i * 2]) << 8) | ((u16)(buffer[i * 2 + 1]))); puts(" ");
		}
	}

	return 0;
}

#define SECTOR_BYTES_COUNT 512
#define SECTOR_MULDIV 9 /* 2^8 */

int ata_rw_action(int device, int cmd, struct ata_rw_info* rw_info) {
	if ((cmd != ATA_CMD_READ) && (cmd != ATA_CMD_WRITE)) return 1;

	int sectors = rw_info->count / SECTOR_BYTES_COUNT;
	int ost = rw_info->count % SECTOR_BYTES_COUNT;

	if (cmd == ATA_CMD_READ) 
		rw_info->buff = (u8*)malloc(sizeof(u8) * rw_info->count);	

	int i;
	for (i = 0; i < sectors; ++i) {
		int current_offset = i << SECTOR_MULDIV;
		if (ata_rwsect_action(device, cmd, rw_info->lba + current_offset, rw_info->buff + current_offset)) {
			if (cmd == ATA_CMD_READ) 
				free(rw_info->buff); 
			return 1;
		}
	}	

	if (ost != 0) { 
		u8* tmp_buf;

		tmp_buf = (u8*)malloc(sizeof(u8) * SECTOR_BYTES_COUNT);

		if (cmd == ATA_CMD_WRITE) 
			memcpy(tmp_buf, rw_info->buff + (sectors << SECTOR_MULDIV), ost);
		
		if (ata_rwsect_action(device, cmd, rw_info->lba + (sectors << SECTOR_MULDIV), tmp_buf)) {
			if (cmd == ATA_CMD_READ) 
				free(rw_info->buff);							
			free(tmp_buf);

			return 1;
		}
		if (cmd == ATA_CMD_READ) 
			memcpy(rw_info->buff + (sectors << SECTOR_MULDIV), tmp_buf, ost);
		
		free(tmp_buf);
	}

	return 0;
}

int ata_read(int device, struct ata_rw_info* r_info) {
	return ata_rw_action(device, ATA_CMD_READ, r_info);
}

int ata_write(int device, struct ata_rw_info* w_info) {
	return ata_rw_action(device, ATA_CMD_WRITE, w_info);
}

