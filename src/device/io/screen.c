#include<libc/utils.h>
#include<libc/stdlib.h>

unsigned short *textmemptr;
int attrib = 0x0F;
int csr_x = 0, csr_y = 0;

void scroll(void)
{
    unsigned blank, temp;
    blank = 0x20 | (attrib << 8);

    if(csr_y >= 25)
    {
        temp = csr_y - 25 + 1;
        memcpy (textmemptr, textmemptr + temp * 80, (25 - temp) * 80 * 2);
        memsetw (textmemptr + (25 - temp) * 80, blank, 80);
        csr_y = 25 - 1;
    }
}

void move_csr(void)
{
    unsigned temp;
    temp = csr_y * 80 + csr_x;
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

void cls()
{
    unsigned blank;
    int i;

    blank = 0x20 | (attrib << 8);
    for(i = 0; i < 25; i++)
        memsetw (textmemptr + i * 80, blank, 80);
    csr_x = 0;
    csr_y = 0;
    move_csr();
}

void putch(unsigned char c)
{
    unsigned short *where;
    unsigned att = attrib << 8;

    if(c == 0x08)
    {
        if(csr_x != 0) csr_x--;
    }
    else if(c == 0x09)
    {
        csr_x = (csr_x + 8) & ~(8 - 1);
    }
    else if(c == '\r')
    {
        csr_x = 0;
    }
    else if(c == '\n')
    {
        csr_x = 0;
        csr_y++;
    }
    else if(c >= ' ')
    {
        where = textmemptr + (csr_y * 80 + csr_x);
        *where = c | att;
        csr_x++;
    }
    if(csr_x >= 80)
    {
        csr_x = 0;
        csr_y++;
    }

    scroll();
    move_csr();
}

void puts(unsigned char *text)
{
    int i;

    for (i = 0; i < strlen(text); i++)
    {
        putch(text[i]);
    }
}

void settextcolor(unsigned char forecolor, unsigned char backcolor)
{
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}

void init_video(void)
{
    textmemptr = (unsigned short *)0xB8000;
    cls();
}

void put_int(int a) {
        if (a == 0)  {
		puts("0\0");
		return;
	}
                
				char* mystr = (char*)malloc(sizeof(char) * 100);
        int pos = 0;
        if (a < 0) {
                mystr[pos++] = '-';
                a = -a;
        }
                                                                                        
//        int st[100];
				int* st = (int*)malloc(sizeof(int) * 100);
        int stacklen = 0;
        for (;a != 0;) {
                int tmp = 0;
                tmp = a % 10;
                a -= tmp;
                a /= 10;
                                                                                                                                        
                st[stacklen++] = tmp;        
        }
                                                                                                                                                                                                                                                                        
        for (a = stacklen-1; a >= 0; a--) 
                        mystr[pos++] = (char)(st[a]+48); 

                                                                                                                                                                                                                                                                                                        
        mystr[pos] = '\0';

        puts(mystr);
}
