
#include "graphic.h"
#include "bootinfo.h"
#include "hankaku.h"

static void set_pallet( int start, int end, unsigned char* rgb )
{
    int i, eflags;
    eflags = _io_load_eflags();    // 割り込み許可フラグの値を記録
    _io_cli();    // 割り込み禁止

    _io_out8( 0x03C8, start );
    for( i = start; i <= end; ++i ){
        _io_out8( 0x03C9, rgb[0] / 4 );
        _io_out8( 0x03C9, rgb[1] / 4 );
        _io_out8( 0x03C9, rgb[2] / 4 );
        rgb += 3;
    }
    _io_store_eflags( eflags );    // 割り込み許可
}

void init_pallet(void)
{
    static unsigned char skTableRGB[16*3] = {
        0x00, 0x00, 0x00,    // black
        0xFF, 0x00, 0x00,    // red
        0x00, 0xFF, 0x00,    // green
        0xFF, 0xFF, 0x00,    // yellow
        0x00, 0x00, 0xFF,    // blue
        0xFF, 0x00, 0xFF,    // purple
        0x00, 0xFF, 0xFF,    // light blue
        0xFF, 0xFF, 0xFF,    // white
        0xc6, 0xc6, 0xc6,    // light gray
        0x84, 0x00, 0x00,    // dark red
        0x00, 0x84, 0x00,    // dark green
        0x84, 0x84, 0x00,    // dark yellow
        0x00, 0x00, 0x84,    // dark blue
        0x84, 0x00, 0x84,    // dark purple
        0x00, 0x84, 0x84,    // deep blue
        0x84, 0x84, 0x84,    // dark gray
    };

    set_pallet( 0, 15, skTableRGB );
}


void boxfill8( int x0, int y0, int x1, int y1, uint8_t color )
{
    int x, y;
    const BootInfo* binfo = get_bootinfo();
    int scrnx = binfo->scrnx;
    int scrny = binfo->scrny;
    uint8_t* vram = binfo->vram;

    for( y = y0; y <= y1 && y < scrny; ++y ){
        for( x = x0; x <= x1 && x < scrnx; ++x ){
            if( x >= 0 && y >= 0 ){
                vram[y * scrnx + x] = color;
            }
        }
    }
}

void putfont8( int x0, int y0, uint8_t color, uint8_t c )
{
    int x, y;
    const BootInfo* binfo = get_bootinfo();
    int scrnx = binfo->scrnx;
    int scrny = binfo->scrny;
    uint8_t* vram = binfo->vram;

    uint8_t t, mask;
    uint8_t* bitmap = get_fontbitmap( c );

    for( y = y0; y < y0+16 && y < scrny; ++y ){
        t = bitmap[y - y0];
        mask = (1 << 7);
        for( x = x0; x < x0+8 && x < scrnx; ++x ){
            if( x >= 0 && y >= 0 && (t & mask) ){
                vram[y * scrnx + x] = color;
            }
            mask >>= 1;
        }
    }
}

Point putstr8( int x0, int y0, uint8_t color, const uint8_t* s )
{
    Point p;

    int x = x0;
    int y = y0;
    while( *s != 0 ){
        if( *s == '\n' ){
            y += 16;
            x = x0;
            ++s;
            continue;
        }

        putfont8( x, y, color, *s );
        x += 8;
        ++s;
    }

    p.x = x; p.y = y;
    return p;
}

void draw_cursor( int x0, int y0, const uint8_t* cursor )
{
    int x, y;
    const BootInfo* binfo = get_bootinfo();
    int scrnx = binfo->scrnx;
    int scrny = binfo->scrny;
    uint8_t* vram = binfo->vram;

    for( y = y0; y < y0+16 && y < scrny; ++y ){
        for( x = x0; x < x0+16 && x < scrnx; ++x ){
            if( x >= 0 && y >= 0 ){
                vram[y * scrnx + x] = cursor[(y-y0)*16 + (x-x0)];
            }
        }
    }
}

