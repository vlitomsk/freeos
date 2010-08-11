#ifndef FAT32_H_INCLUDED
#define FAT32_H_INCLUDED

#include <libc/stdlib.h>

#define FAT32_NAME_SEPARATOR '/'

#define FAT32_MEDIA_HDD 0xF8
#define FAT32_MEDIA_REM 0xF0

#define FAT32_DRVNUM_HD 0x80
#define FAT32_DRVNUM_FD 0x0

struct fat32_header {
	u8 jmpboot[3];
	char oem_name[8]; 
	u16 bytes_per_sect; // 512
	u8 sect_per_clus; // 1..128 (!2^n)
	u16 resvd_sect_cnt; // 0x20
	u8 fats_num; // 2
	u16 root_ent_cnt; // 0
	u16 tot_sect16; // 0
	u8 media; // FAT32_MEDIA_... !! Важно, чтобы это же значение было записано в байт таблицы FAT[0]. !! (из спека)
	u16 fat_sz16; // 0
	u16 sect_per_trk; // 0 , фтопку, для CHS, int 0x13
	u16 num_heads; // 0 , тоже фтопку
	u32 hidd_sect; // 0
	u32 tot_sect32; // количество секторов в разделе fat. (чертовы девелоперы спека, я 15 минут угробил,чтоб понять, что "диск" и "раздел FAT" - одно и то же :))
	u32 fat_sz32; // количество секторов в одной fat
	u16 ext_flags; // !в спек!
	u16 fs_ver; // 0
	u32 root_clus; // 2
	u16 fs_info; // 1
	u16 bk_bootsect; // 6
	u32 reserved[3]; // 0, 0, 0
	u8 drv_num; // FAT32_DRVNUM_..
	u8 reserved_1; // 0
	u8 boot_sig; // 0x29
	u32 vol_id; // хрень какая-то. можно туда рандомное значение. 
	char vol_lab[11]; // метка. просто строка рандомная. // если нет названия, то "NO NAME    "
	char fil_systype[8]; // "FAT32   "
	//!! ЕЩЕ 0x55AA !!
};

struct fat_t32 {
	u32 len;
	u32* fat;
};

struct fs_fat32 {
	struct fat32_header header;
	u32 lba_start;
	u32 sectors;
	u32 fat1_starts;
	u32 fat2_starts;
	struct fat_t32 fat;
	u32 clus_starts;
};

#define FAT32_ATTR_RO 0x1
#define FAT32_ATTR_HIDD 0x2
#define FAT32_ATTR_SYS 0x4
#define FAT32_ATTR_VOLID 0x8
#define FAT32_ATTR_DIR 0x10
#define FAT32_ATTR_ARCH 0x20
#define FAT32_ATTR_LONGNAME (FAT32_ATTR_RO | FAT32_ATTR_HIDD | FAT32_ATTR_SYS | FAT32_ATTR_VOLID)

extern int mkfs_fat32(int device, int from_mbr, u8 part_n, u32 lba_start, u8 sect_per_clus, u8 media, u32 tot_sect32, u32 fat_sz32, u8 drv_num, u32 vol_id, u8* vol_lab, struct fs_fat32* fs);

#endif
