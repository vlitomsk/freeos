#ifndef PCI_H
#define PCI_H

#define PCI_CONF_ADDR 0xCF8
#define PCI_CONF_DATA 0xCFC

// максимальное количество
#define PCI_BUS_COUNT 0x100
#define PCI_DEV_COUNT 0x20
#define PCI_FUN_COUNT 0x8

#include <libc/stdlib.h>

struct pci_device {
    u16 vendor_id;
	u16 device_id;
    u32 class_code;
    u32 base_addr; // bar0..5 , но у которого 0бит взведен
    u8 bus, dev, func;
};

extern u32 pci_read_confspace(u8 bus, u8 dev, u8 func, u8 reg);
// на самом деле ccode - это класс коде :)
extern int pci_init_device(int startbus, int startdev, int startfunc, u32 ccode, struct pci_device* dev_);

#endif
