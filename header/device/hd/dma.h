#ifndef DMA_H
#define DMA_H

#include<libc/stdlib.h>
#define TRANSTYPE_WRITE 1
#define TRANSTYPE_READ 2

#define MOD_SINGLE 1
#define MOD_BLOCK 2 // пока не стоит его использовать.

// page address register
#define PAR_0 0x87
#define PAR_1 0x83
#define PAR_2 0x81
#define PAR_3 0x82

/*
 *  самые "конечные функции" - dma_prepare_read , dma_prepare_write.
 * chan - канал (0-3), 16-битные пока не обрабатываются
 * start_addr - стартовый адрес(!!на устройстве)
 * data_len - сколько байт читать/писать c/в устройство(а)
 * !!! data_len - на самом деле длина-1, допустим
 * если 64кб, то data_len=0xFFFF
 */

 // нужно вызвывать dma_prepare_...
extern int dma_init(u16 start_addr, u16 data_len, u8 chan);
extern void dma_prepare_write(u8 chan, u16 start_addr, u16 data_len);
extern void dma_prepare_read(u8 chan, u16 start_addr, u16 data_len);

/*
extern int is_valid_channel(u8 chan);
extern void mask_channel(u8 chan);
extern void unmask_channel(u8 chan);
extern void flipflop_reset(void); // для 16-битных портов
extern void send_word(u16 port, u16 word);
extern void send_addr(u16 addr);
extern void send_count(u16 len);
extern void set_mode_reg(u8 sel, u8 tra, u8 _auto, u8 down, u8 mod);
*/

#endif
