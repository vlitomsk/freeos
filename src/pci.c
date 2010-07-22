#include <pci.h>
#include <stdlib.h>
#include <utils.h>

u32 mk_confaddr(u8 bus, u8 dev, u8 func, u8 reg) {	
	u32 result = 0;
	result = ((((u32)(bus & 0xFF)) << 16) | (((u32)(dev & 0xF)) << 11) | (((u32)(func & 0x3)) << 8) | (((u32)(reg & 0x3F)) & ~3) | 0x80000000); // LISP нервно курит в сторонке :-)

	return result; 
}

u32 pci_read_confspace(u8 bus, u8 dev, u8 func, u8 reg) {
	outportl(PCI_CONF_ADDR, mk_confaddr(bus, dev, func, reg));
	return inportl(PCI_CONF_DATA);
}


int pci_init_device(int startbus, int startdev, int startfunc, u32 ccode, struct pci_device* dev_) {
	int bus, dev, fun;
	for (bus = startbus; bus < PCI_BUS_COUNT; ++bus) {
		for (dev = startdev; dev < PCI_DEV_COUNT; ++dev) {
			for (fun = startfunc; fun < PCI_FUN_COUNT; ++fun) {				
				if ((u32)(pci_read_confspace(bus, dev, fun, 8) >> 8) == ccode) {
					// ура!!
					u32 tmp = pci_read_confspace(bus, dev, fun, 0);
					dev_->vendor_id = tmp & 0xFFFF;
					dev_->device_id = tmp >> 16;
					dev_->class_code = ccode;
					int bar_off; 
					for (bar_off = 0x10; bar_off < 0x28; bar_off += 0x4) {
						tmp = pci_read_confspace(bus, dev, fun, bar_off);	
						if (tmp & 0x1) {
							tmp &= ~0x1;
							bar_off = -1;
							break;
						}
					}
					if (bar_off == -1) {
						// вдвойне ура!!
						dev_->base_addr = tmp;
					} else continue;
					dev_->bus = bus;
					dev_->dev = dev; // :)
					dev_->func = fun;

					return 0;
				}			
			}
		}
	}

	return -1;
}

/*
 * Keep it simple, stupid! :-)
 */
