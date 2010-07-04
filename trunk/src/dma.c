// Во. Сам написал :-)
#include <dma.h>
#include <utils.h>

int is_valid_channel(u8 chan) {
    if ((chan > 3)) return 1; // пока 8-битные каналы. потом будут 16-.

    return 0;
}

void mask_channel(u8 chan) {
    if (!(is_valid_channel(chan))) return;
    u8 c = 0 | chan | 4;

    outportb(0x0A, c);
}

void unmask_channel(u8 chan) {
    if (!(is_valid_channel(chan))) return;
    u8 c = 0 | chan;

    outportb(0x0A, c);
}

void flipflop_reset(void) {
    outportb(0x0C, 0xFF);
}

void send_word(u16 port, u16 word) {
    outportb(port, LOW_BYTE(word));
    outportb(port, HIGH_BYTE(word));

    flipflop_reset(); // послали слово
}

void send_addr(u16 addr) {
    send_word(0x04, addr);
}

void send_count(u16 len) {
    send_word(0x05, len);
}

int set_mode_reg(u8 sel, u8 tra, u8 _auto, u8 down, u8 mod) {
    if (!(is_valid_channel(sel))) return 1;
    if ((tra != TRANSTYPE_READ) || (tra != TRANSTYPE_WRITE)) return 1;
    tra <<= 2;
    if (_auto > 1) return 1;
    _auto <<= 4;
    if (down > 1) return 1;
    down <<= 5;
    if ((mod != MOD_BLOCK) || (mod != MOD_SINGLE)) return 1;
    mod <<= 6;

    u8 reg_value = 0 | sel | tra | _auto | down | mod;

    outportb(0x0B, reg_value);

    return 0;
}

int dma_prepare_rw(u8 chan, u16 start_addr, u16 data_len, u8 transtype) {
    int code = 0;
    if (dma_init(start_addr, data_len, chan)) code = 1;

    mask_channel(chan);
    if (set_mode_reg(chan, transtype, 1, 0, MOD_SINGLE)) code = 1;
    unmask_channel(chan);

    return code;
}

void dma_prepare_write(u8 chan, u16 start_addr, u16 data_len) {    
    dma_prepare_rw(chan, start_addr, data_len, TRANSTYPE_WRITE);
}

void dma_prepare_read(u8 chan, u16 start_addr, u16 data_len) {
    dma_prepare_rw(chan, start_addr, data_len, TRANSTYPE_READ);
}

int dma_init(u16 start_addr, u16 data_len, u8 chan) {
    if (!(is_valid_channel(chan))) return 1;

    mask_channel(chan);
    flipflop_reset();
    send_addr(start_addr);
    send_count(data_len);
    u16 port;
    switch (chan) {
        case 0: port = PAR_0;
            break;
        case 1: port = PAR_1;
            break;
        case 2: port = PAR_2;
            break;
        case 3: port = PAR_3;
            break;
    }
    outportb(port, 0); // ??
    unmask_channel(chan);

    return 0;
}


