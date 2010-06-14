#include<utils.h>

struct idt_entry
{
    unsigned short base_lo;
    unsigned short sel;      
    unsigned char always0;  
    unsigned char flags;      
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));


struct idt_entry idt[256];
struct idt_ptr idtp;

extern void _idt_load();

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{}

void idt_install()
{
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = &idt;
    memset(&idt, 0, sizeof(struct idt_entry) * 256);
    _idt_load();
}
