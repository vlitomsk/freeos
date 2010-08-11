#include <device/hd/fat32.h>
#include <libc/stdlib.h>
#include <libc/utils.h>
#include <device/hd/ata.h>
#include <libc/string.h>

struct fat32_header mk_fat32_header(u8 sect_per_clus, u8 media, u32 tot_sect32, u32 fat_sz32, u8 drv_num, u32 vol_id, u8* vol_lab) {
	struct fat32_header r;

	r.jmpboot[0] = 0xEB;
	r.jmpboot[1] = 0x00; // ??
	r.jmpboot[2] = 0x90;

//	r.oem_name = ;
	r.bytes_per_sect = 512;
	r.sect_per_clus = sect_per_clus;
	r.resvd_sect_cnt = 0x20;
	r.fats_num = 2;
	r.root_ent_cnt = 0;
	r.tot_sect16 = 0;
	r.media = media;
	r.fat_sz16 = 0;
	r.sect_per_trk = 0;
	r.num_heads = 0;
	r.hidd_sect = 0;
	r.tot_sect32 = tot_sect32;
	r.fat_sz32 = fat_sz32;	
	r.ext_flags = 0; // в спеке про него почитать. но с 0 тоже нормально
	r.fs_ver = 0;
	r.root_clus = 2;
	r.fs_info = 1;
	r.bk_bootsect = 6;
	r.reserved[0] = 0;
	r.reserved[1] = 0;
	r.reserved[2] = 0;
	r.drv_num = drv_num;
	r.reserved_1 = 0;
	r.boot_sig = 0x29;
	r.vol_id = vol_id;
//	if (vol_lab != NULL) 
//		memcpy(r.vol_lab, vol_lab, 11);
//	else
//		r.vol_lab = "NO NAME    ";
//	r.fil_systype = "FAT32   ";

	return r;
}

u8* mk_fat32_bootsector(struct fat32_header h) { // Лучше вовнутрь не глядеть :)
	u8* r = (u8*)malloc(sizeof(u8) * 512);
	int i;
	for (i = 0; i < 512; ++i) 
		r[i] = 0;

	memcpy(r, h.jmpboot, 3);
	memcpy(r + 3, h.oem_name, 8);
	r[12] = 2;
	r[13] = h.sect_per_clus == 128 ? 64 : h.sect_per_clus;
	r[14] = 32;
	r[16] = 2;
	r[21] = h.media;
	r[32] = h.tot_sect32 & 0xFF;
	r[33] = (h.tot_sect32 & 0xFF00) >> 8;
	r[34] = (h.tot_sect32 & 0xFF0000) >> 16;
	r[35] = (h.tot_sect32 & 0xFF000000) >> 24;
	r[36] = h.fat_sz32 & 0xFF;
	r[37] = (h.fat_sz32 & 0xFF00) >> 8;
	r[38] = (h.fat_sz32 & 0xFF0000) >> 16;
	r[39] = (h.fat_sz32 & 0xFF000000) >> 24;
	r[40] = (h.ext_flags & 0xFF);
	r[41] = (h.ext_flags & 0xFF00) >> 8;
	r[44] = 2;
	r[48] = 1;
	r[50] = 6;
	r[64] = h.drv_num;
	r[66] = 0x29;
	r[67] = h.vol_id & 0xFF;
	r[68] = (h.vol_id & 0xFF00) >> 8;
	r[69] = (h.vol_id & 0xFF0000) >> 16;
	r[70] = (h.vol_id & 0xFF000000) >> 24;
	memcpy(r + 71, h.vol_lab, 11);
	memcpy(r + 82, h.fil_systype, 8);

	r[510] = 0x55; // надо так
	r[511] = 0xAA;

	return r;
}

int mkfs_fat32(int device, int from_mbr, u8 part_n, u32 lba_start, u8 sect_per_clus, u8 media, u32 tot_sect32, u32 fat_sz32, u8 drv_num, u32 vol_id, u8* vol_lab, struct fs_fat32* fs) {
	if (from_mbr) {
		int part_mbroffset = 448 + (part_n - 1) * 18;
		struct ata_rw_info pinfo;
		pinfo.lba = part_mbroffset;
		pinfo.count = 16;
		if (ata_read(device, &pinfo)) return 1;
		lba_start = (u32)(pinfo.buff[8]) | ((u32)(pinfo.buff[9]) >> 8) | ((u32)(pinfo.buff[10]) >> 16) | ((u32)(pinfo.buff[11]) >> 24);
		tot_sect32 = (u32)(pinfo.buff[12]) | ((u32)(pinfo.buff[13]) >> 8) | ((u32)(pinfo.buff[14]) >> 16) | ((u32)(pinfo.buff[15]) >> 24);
		free(pinfo.buff);
	}

	int i;

	struct ata_rw_info tmp;
	tmp.lba = lba_start;
	tmp.count = 512;
	tmp.buff = mk_fat32_bootsector(mk_fat32_header(sect_per_clus, media, tot_sect32, fat_sz32, drv_num, vol_id, vol_lab));
	if (ata_write(device, &tmp)) {
		free(tmp.buff);
		return 1;
	}

	free(tmp.buff);

	struct fat_t32 fat1;
	fat1.len = fat_sz32 * 128; // magic-magic, magic-magiiic
	fat1.fat = (u32*)malloc(sizeof(u32) * fat1.len);
	for (i = 0; i < fat_sz32 * 128; ++i) 
		fat1.fat[i] = 0;	
	struct fat_t32 fat2;
	fat2.len = fat_sz32 * 128;
	fat2.fat = (u32*)malloc(sizeof(u32) * fat2.len);

	fat1.fat[0] = media;
	u32 fat1_starts = lba_start + 33;
	u32 fat2_starts = fat1_starts + 512 * fat_sz32;
	u32 clus_starts = fat2_starts + 512 * fat_sz32;

	fat1.fat[2] = 0xFFFFFFFF;	
	
	for (i = 0; i < fat_sz32 * 128; ++i) 
		fat2.fat[i] = fat1.fat[i];

	tmp.lba = fat1_starts;
	tmp.count = fat_sz32;
	tmp.buff = (u8*)malloc(sizeof(u8) * fat_sz32 * 512);
	for (i = 0; i < fat_sz32 * 128; ++i) {
		tmp.buff[i * 4] = fat1.fat[i] & 0xFF;
		tmp.buff[i * 4 + 1] = (fat1.fat[i] & 0xFF00) >> 8;
		tmp.buff[i * 4 + 2] = (fat1.fat[i] & 0xFF0000) >> 16;
		tmp.buff[i * 4 + 3] = (fat1.fat[i] & 0xFF000000) >> 24;
	}
	if (ata_write(device, &tmp)) {
		free(tmp.buff);
		return 1;
	}
	tmp.lba = fat2_starts;
	if (ata_write(device, &tmp)) {
		free(tmp.buff);
		return 1;
	}

	free(tmp.buff);

	fs->lba_start = lba_start;
	fs->sectors = tot_sect32;
	fs->fat1_starts = fat1_starts;
	fs->fat2_starts = fat2_starts;
	fs->clus_starts = clus_starts;

	return 0;
}

int write_fats(int device, struct fs_fat32 fs) {
	int i;
	for (i = 0; i < fs.fat.len; ++i) {
		struct ata_rw_info tmp;
		tmp.lba = fs.fat1_starts + i * 4;
		tmp.count = 4;
		u8 buff[4];
		tmp.buff = buff;
		buff[0] = fs.fat.fat[i] & 0xFF;
		buff[1] = (fs.fat.fat[i] & 0xFF00) >> 8;
		buff[2] = (fs.fat.fat[i] & 0xFF0000) >> 16;
		buff[3] = (fs.fat.fat[i] & 0xFF000000) >> 24;

		while (ata_write(device, &tmp)) {};
	}

	return 0;
}

u32 find_fentry(struct fs_fat32 fs, u32 offset, u32 val) {
	u32 i;
	if (offset < 3) return 0;
	for (i = offset; i < fs.fat.len; ++i) 
		if (fs.fat.fat[i] == val) return i;	

	return 0;
}

u32 find_free_fentry(struct fs_fat32 fs, u32 offset) {
	return find_fentry(fs, offset, 0);
}

u32 get_entity_cluster(int device, char** path, int pathlen, struct fs_fat32* fs, u32 start_cluster, u8* succ, int lev) {
	lev++;
/*	if (streql(path, "/")) {
		*succ = 1;
		return 2;
	}*/
	if (pathlen == 0) { // если будеь ошибка,то,возможно,она будет и тут
		*succ = 0;
		return 0;
	}
	int i;
	for (i = start_cluster; i != 0xFFFFFFFF; i = (fs->fat).fat[i]) {
		struct ata_rw_info tmp;
		tmp.lba = fs->clus_starts + i * (fs->header).sect_per_clus * 512;
		tmp.count = 32;
		if (ata_read(device, &tmp)) {
			*succ = 0;	
			return 0;
		}
		char* s_tmp = (char*)malloc(sizeof(char) * 11);
		memcpy(s_tmp, tmp.buff, 11);
		if ((pathlen == 1) && (streqln(path[0], s_tmp, strlen(path[0])))) {
			*succ = 1;
			free(s_tmp);
			free(tmp.buff);
			return i;
		} else if (pathlen == 1) {
			*succ = 0;
			free(s_tmp);
			free(tmp.buff);	
			return 0;
		}

		if ((~((tmp.buff[11] & FAT32_ATTR_DIR) >> 4))) {
			free(tmp.buff);
			free(s_tmp);
			continue;
		}
		
		if (streqln(path[lev - 1], s_tmp, strlen(path[lev - 1]))) {
			free(s_tmp);
			free(tmp.buff);
			return get_entity_cluster(device, path + 1, pathlen - 1, fs, i, succ, lev);
		}

		free(s_tmp);
		free(tmp.buff);
	}

	*succ = 0;
	return 0;
}

u32 get_last_chain_cluster(u32 cluster_from_chain, struct fs_fat32* fs) { // который с 0xFFFFFFFF
	int i;
	for (i = cluster_from_chain; i != 0xFFFFFFFF; i = (fs->fat).fat[i]) {};
	return i;
}

char** get_path_parts(char* path, int* result_len) {
	int* plen;	
	char** result = split(path, result_len, plen, FAT32_NAME_SEPARATOR);
	int i;
	for (i = 0; i < *result_len; ++i) 
		upcase(result[i]);		
	
	free(plen);

	return result;
}

u8* read_cluster_sign(int device, struct fs_fat32 fs, u32 clus) {
	struct ata_rw_info tmp;
	tmp.lba = fs.clus_starts + (clus << 9) * fs.header.sect_per_clus;
	tmp.count = 32;
	if (ata_read(device, &tmp) == 0) 
		return tmp.buff;
	
	return NULL;
}

int write_cluster_sign(int device, struct fs_fat32 fs, u32 clus, u8* sign) {
	struct ata_rw_info tmp;
	tmp.lba = fs.clus_starts + (clus << 9) * fs.header.sect_per_clus;
	tmp.count = 32;
	tmp.buff = sign;
	
	return ata_write(device, &tmp);
}

#define sign_name(sign, name) memcpy(name, sign, 11)
#define sign_attr(sign) (*(sign + 11))
#define sign_fstclus(sign) ((sign[26]) | (((u32)(sign[27])) >> 8) | (((u32)(sign[20])) >> 16) | (((u32)(sign[20])) >> 24))
#define sign_filesize(sign) ((sign[28]) | (((u32)(sign[29])) >> 8) | (((u32)(sign[30])) >> 16) | (((u32)(sign[31])) >> 24))

void sign_mkname(u8* sign, char* name) {
	memcpy(sign, name, 11);
}

void sign_mkattr(u8* sign, u8 attr) {
	sign[11] = attr;
}

void sign_mkfstclus(u8* sign, u32 fstclus) {
	sign[26] = (fstclus & 0xFF);
	sign[27] = (fstclus & 0xFF00) >> 8;
	sign[20] = (fstclus & 0xFF0000) >> 16;
	sign[21] = (fstclus & 0xFF000000) >> 24;
}

void sign_mkfilesize(u8* sign, u32 newsz) {
	sign[28] = (newsz & 0xFF);
	sign[29] = (newsz & 0xFF00) >> 8;
	sign[30] = (newsz & 0xFF0000) >> 16;
	sign[31] = (newsz & 0xFF000000) >> 24;
}

int fat32_mkentity(int device, struct fs_fat32* fs, char* path, u8 attr) { // сущность. кто писал на ogre, поймет :-)
	int cnt;
	char** pparts = get_path_parts(path, &cnt);
	u8 succ;
	u32 direntr = get_entity_cluster(device, pparts, cnt - 1, fs, 2, &succ, 0);
	if (succ == 0) {
		int i;
		for (i = 0; i < cnt; ++i) free(pparts[i]);
		free(pparts);

		return 1;
	}
	u8* sign;
	sign = read_cluster_sign(device, *fs, direntr);
	int retval = 0;
	if (sign != NULL) {
		if (sign_fstclus(sign) == 0xFFFFFFFF) {
			u32 svob = find_free_fentry(*fs, 2);
			(fs->fat).fat[svob] = 0xFFFFFFFF;
			sign_mkfstclus(sign, svob);
			if (write_cluster_sign(device, *fs, direntr, sign)) {
				int i;
				for (i = 0; i < cnt; ++i) free(pparts[i]);
				free(pparts);

				return 1;
			}
			
			char sign2[32];
			sign_mkname(sign2, pparts[cnt - 1]);
			sign_mkattr(sign2, attr);
			sign_mkfstclus(sign2, 0xFFFFFFFF);
			if (write_cluster_sign(device, *fs, svob, sign2)) {
				int i;
				for (i = 0; i < cnt; ++i) free(pparts[i]);
				free(pparts);

				return 1;
			}
		} else {
			u32 lastcl = get_last_chain_cluster(sign_fstclus(sign), fs);
			u32 svob = find_free_fentry(*fs, 2);
			(fs->fat).fat[svob] = 0xFFFFFFFF;
			(fs->fat).fat[lastcl] = svob;
			
			char sign2[32];
			sign_mkname(sign2, pparts[cnt - 1]);
			sign_mkattr(sign2, attr);
			sign_mkfstclus(sign, 0xFFFFFFFF);
			if (write_cluster_sign(device, *fs, svob, sign2)) {
				int i;
				for (i = 0; i < cnt; ++i) free(pparts[i]);
				free(pparts);

				return 1;	
			}
		}
	} else retval = 1;

	int i;
	for (i = 0; i < cnt; ++i) free(pparts[i]);
	free(pparts);

	write_fats(device, *fs);

	return retval;
}

int fat32_mkdir(int device, struct fs_fat32* fs, char* path, u8 attr) {
	return fat32_mkentity(device, fs, path, attr | FAT32_ATTR_DIR);
}

int fat32_mkfile(int device, struct fs_fat32* fs, char* path, u8 attr) {
	return fat32_mkentity(device, fs, path, attr & (~FAT32_ATTR_DIR));
}

char** fat32_read_dir(int device, struct fs_fat32* fs, char* path, int* length) {
	u32 cnt;
	char** pparts = get_path_parts(path, &cnt);
	
	u8 succ;
	u32 dirclus =  get_entity_cluster(device, pparts, cnt, fs, 2, &succ, 0);
	
	int i;
	for (i = 0; i < cnt; ++i) free(pparts[i]);
	free(pparts);

	u8* sign;
	sign = read_cluster_sign(device, *fs, dirclus);
	if (sign_fstclus(sign) == 0xFFFFFFFF) return NULL;

	*length = 0;
	for (i = sign_fstclus(sign); ; i = (fs->fat).fat[i]) {
		(*length)++;
		if (i == 0xFFFFFFFF) break;
	}

	char** result = (char**)malloc(sizeof(char*) * (*length));
	for (i = 0; i < (*length); ++i) 
		result[i] = (char*)malloc(sizeof(char) * 11);

	cnt = 0;
		
	for (i = sign_fstclus(sign); ; i = (fs->fat).fat[i]) {
		u8* tmp_sign;
		tmp_sign = read_cluster_sign(device, *fs, i);
		sign_name(tmp_sign, result[cnt]);
		cnt++;
		if (cnt == *length) break;
	}

	return result;
}

char* fat32_file_part(int device, struct fs_fat32* fs, char* path, int* length, int offset, int byte_cnt) {
	int cnt;
	char** pparts = get_path_parts(path, &cnt);
	
	u8 succ;
	u32 fclus =  get_entity_cluster(device, pparts, cnt, fs, 2, &succ, 0);
	
	int i;
	for (i = 0; i < cnt; ++i) free(pparts[i]);
	free(pparts);

	u8* sign;
	sign = read_cluster_sign(device, *fs, fclus);

	if (byte_cnt > sign_filesize(sign)) 
		*length = sign_filesize(sign) - offset;
	else
		*length = byte_cnt;

	char* result = (char*)malloc(sizeof(char) * (*length));

	offset += 32; // сигнатуры
	u32 startclus = sign_fstclus(sign);
	startclus += offset / ((fs->header).sect_per_clus << 9);
	int j = 0;
	
	for (i = 0; i <= *length;) {
		struct ata_rw_info tmp;
		u8 br = 0;
		if (i == 0) {
			tmp.lba = fs->clus_starts + (startclus * (fs->header).sect_per_clus) << 9 + offset % ((fs->header).sect_per_clus << 9);
			tmp.count = ((startclus + 1) * (fs->header).sect_per_clus) << 9 - offset % ((fs->header).sect_per_clus << 9);
		} else if (*length - i >= ((fs->header).sect_per_clus << 9)) {
			tmp.lba = fs->clus_starts + j * ((fs->header).sect_per_clus << 9);
			tmp.count = (fs->header).sect_per_clus << 9;
		} else {
			tmp.lba = fs->clus_starts + j * ((fs->header).sect_per_clus << 9);
			tmp.count = (j + 1) * ((fs->header).sect_per_clus << 9) - i;							
		}

		j++;
		i += tmp.count;

		if (ata_read(device, &tmp)) {
			free(result);
			return NULL;
		}

		memcpy(result + i, tmp.buff, tmp.count);
	}

	return result;
}

void fat32_delchain(struct fs_fat32* fs, u32 start) {
	u32 i;
//	u32 next = (fs->fat).fat[start];
	u32 next;
	for (i = start; i != 0xFFFFFFFF; i = next) {
		next = (fs->fat).fat[i];
		(fs->fat).fat[i] = 0;
	}
}

void fat32_delentity_(int device, struct fs_fat32* fs, u32 clus) { // !!ДОПИСАТЬ!
	u8* sign;
	sign = read_cluster_sign(device, *fs, clus);
	if (sign_fstclus(sign) == 0xFFFFFFFF) { (fs->fat).fat[clus] = 0; return; }
	u32 i;
	u32 next = (fs->fat).fat[sign_fstclus(sign)];
	for (i = sign_fstclus(sign); ; i = next) {
		next = (fs->fat).fat[i];

		u8* tmp_sign;
		tmp_sign = read_cluster_sign(device, *fs, i);
		if ((sign_attr(tmp_sign) & FAT32_ATTR_DIR) >> 4)
			fat32_delentity_(device, fs, i);
		if (~((sign_attr(tmp_sign) & FAT32_ATTR_DIR) >> 4))
			fat32_delchain(fs, i);

		if ((fs->fat).fat[i] == 0xFFFFFFFF) break;
	}
	// сигнатуру тут перезаписать с 0xFFFFFFFF
	sign_mkfstclus(sign, 0xFFFFFFFF);
	write_cluster_sign(device, *fs, clus, sign);
}

int fat32_delentity(int device, struct fs_fat32* fs, char* path) {
	write_fats(device, *fs);
}

int fat32_appendfile(int device, struct fs_fat32* fs, char* path, char* data, int datalen) {
	int cnt;
	char** parts = get_path_parts(path, &cnt);
	int i;
	if (parts == NULL) 
		return 1;	
	
	u8 tmp_flag;
	u32 headc = get_entity_cluster(device, parts, cnt, fs, 2, &tmp_flag, 0);
	if (tmp_flag == 0) {
		for (i = 0; i < cnt; ++i) 
			free(parts[i]);
		free(parts);

		return 1;
	}
	u8* sign;	
	sign = read_cluster_sign(device, *fs, headc);
	tmp_flag = 0;
	if (sign_filesize(sign) + 32 < ((fs->header).sect_per_clus << 9)) // дописывать в головной кластер
		tmp_flag = 1;

	u32 clusters = 0;
	u32 ost = 0;

	if (tmp_flag == 0) {
		clusters = datalen / ((fs->header).sect_per_clus << 9);
		ost = datalen % ((fs->header).sect_per_clus << 9);
	}

	u32 cclus = 1;

	for (i = 0; (datalen > 0); ) {
		struct ata_rw_info tmp;
		if (tmp_flag) {
			tmp.lba = fs->clus_starts + headc * ((fs->header).sect_per_clus << 9) + sign_filesize(sign);
			tmp.count = ((fs->header).sect_per_clus << 9) - sign_filesize(sign);
			tmp.buff = data;
			if (ata_write(device, &tmp)) {
				for (i = 0; i < cnt; ++i)
					free(parts[i]);
				free(parts);

				return 1;
			}
			i += tmp.count;
			datalen -= tmp.count;

			clusters = datalen / ((fs->header).sect_per_clus << 9);
			ost = datalen % ((fs->header).sect_per_clus << 9);

			tmp_flag = 0;
			continue;
		}

		u32 last = get_last_chain_cluster(headc, fs);
		u32 svob = find_free_fentry(*fs, 2);

		(fs->fat).fat[svob] = 0xFFFFFFFF;
		(fs->fat).fat[last] = svob;

		tmp.lba = fs->clus_starts + (svob) * ((fs->header).sect_per_clus << 9);
		tmp.count = (fs->header).sect_per_clus << 9; 
		tmp.buff = data + i;

		if (clusters == 0) { // записываем последний кластер, точней то, что осталось.			
			tmp.count = ost;
			if (ata_write(device, &tmp)) {
				for (i = 0; i < cnt; ++i)
					free(parts[i]);
				free(parts);

				return 1;
			}

			i += tmp.count;
			datalen -= tmp.count;			

			break;
		}
		
		if (ata_write(device, &tmp)) {
			for (i = 0; i < cnt; ++i) 
				free(parts[i]);
			free(parts);

			return 1;
		}

		clusters--;
		i += tmp.count;
		datalen -= tmp.count;
	}

	// !!! ЗАПИСЬ В FILESIZE !!!!!
	sign_mkfilesize(sign, i);
	write_cluster_sign(device, *fs, headc, sign);
	
	write_fats(device, *fs);

	for (i = 0; i < cnt; ++i)
		free(parts[i]);
	free(parts);

	return 0;
}

