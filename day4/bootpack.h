#ifndef    BOOTPACK_H_INCLUDED
#define    BOOTPACK_H_INCLUDED

void _io_hlt(void);
void _io_cli(void);

void _io_out8( int port, int data );

int _io_load_eflags(void);
void _io_store_eflags( int eflags );

void _write_mem8(void);

typedef struct _BootInfo
{
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char* vram;
} BootInfo;

enum VGAPalletColor    // 0 - 15 16色
{
    COLOR_BLACK = 0, 
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_PURPLE,
    COLOR_LIGHT_BLUE,
    COLOR_WHITE,
    COLOR_LIGHT_GRAY,
    COLOR_DARK_RED,
    COLOR_DARK_GREEN,
    COLOR_DARK_YELLOW,
    COLOR_DARK_BLUE,
    COLOR_DARK_PURPLE,
    COLOR_DEEP_BLUE,
    COLOR_DARK_GRAY
};

#endif
