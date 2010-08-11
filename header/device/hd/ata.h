#ifndef ATA_H_INCLUDED
#define ATA_H_INCLUDED

#include<libc/stdlib.h>
#include<libc/utils.h>
extern void set_channel(int ch);
extern void set_timeout(int t);

#define DEVICE_MASTER 0
#define DEVICE_SLAVE 0x10

struct ata_rw_info {
	u32 lba;
	u32 lba2; // для 48-битного PIO
	u32 count;
	u8* buff; // не надо ничего выделять.
};

extern int ata_read(int device, struct ata_rw_info* r_info);
extern int ata_write(int device, struct ata_rw_info* w_info);

#endif
