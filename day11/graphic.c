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


void boxfill8( Sheet* sht, int x0, int y0, int x1, int y1, uint8_t color )
{
    int x, y;
    int bxsize = sht->bxsize;
    int bysize = sht->bysize;
    uint8_t* vram = sht->bitmap;

    for( y = y0; y <= y1 && y < bysize; ++y ){
        for( x = x0; x <= x1 && x < bxsize; ++x ){
            if( x >= 0 && y >= 0 ){
                vram[y * bxsize + x] = color;
            }
        }
    }
}

void putfont8( Sheet* sht, int x0, int y0, uint8_t color, uint8_t bg_color, uint8_t ch )
{
    int x, y;
    int bxsize = sht->bxsize;
    int bysize = sht->bysize;
    uint8_t* vram = sht->bitmap;

    uint8_t t, mask;
    const uint8_t* bitmap = get_fontbitmap( ch );

    for( y = y0; y < y0+16 && y < bysize; ++y ){
        t = bitmap[y - y0];
        mask = (1 << 7);
        for( x = x0; x < x0+8 && x < bxsize; ++x ){
            if( x >= 0 && y >= 0 ){
                // 文字の部分
                if( (t & mask) ){
                    vram[y * bxsize + x] = color;
                }
                // 文字以外の部分は背面色
                else {
                    vram[y * bxsize + x] = bg_color;
                }
            }
            mask >>= 1;
        }
    }
}

Point putstr8( Sheet* sht, int x0, int y0, uint8_t color, uint8_t bg_color, const uint8_t* s )
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

        putfont8( sht, x, y, color, bg_color, *s );
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

void init_screen( Sheet* sht )
{
    int xsize = sht->bxsize;
    int ysize = sht->bysize;

    boxfill8( sht, 0, 0, 320, 200, COLOR_BLACK );
    boxfill8( sht, 0,         0,          xsize -  1, ysize - 29, COLOR_DEEP_BLUE );
    boxfill8( sht, 0,         ysize - 28, xsize -  1, ysize - 28, COLOR_LIGHT_GRAY );
    boxfill8( sht, 0,         ysize - 27, xsize -  1, ysize - 27, COLOR_WHITE );
    boxfill8( sht, 0,         ysize - 26, xsize -  1, ysize -  1, COLOR_LIGHT_GRAY );
    boxfill8( sht, 3,         ysize - 24, 59,         ysize - 24, COLOR_WHITE );
    boxfill8( sht, 2,         ysize - 24,  2,         ysize -  4, COLOR_WHITE );
    boxfill8( sht, 3,         ysize -  4, 59,         ysize -  4, COLOR_DEEP_BLUE );
    boxfill8( sht, 59,        ysize - 23, 59,         ysize -  5, COLOR_DEEP_BLUE );
    boxfill8( sht, 2,         ysize -  3, 59,         ysize -  3, COLOR_BLACK );
    boxfill8( sht, 60,        ysize - 24, 60,         ysize -  3, COLOR_BLACK );
    boxfill8( sht, xsize - 47, ysize - 24, xsize -  4, ysize - 24, COLOR_DEEP_BLUE );
    boxfill8( sht, xsize - 47, ysize - 23, xsize - 47, ysize -  4, COLOR_DEEP_BLUE );
    boxfill8( sht, xsize - 47, ysize -  3, xsize -  4, ysize -  3, COLOR_WHITE );
    boxfill8( sht, xsize -  3, ysize - 24, xsize -  3, ysize -  3, COLOR_WHITE );
}
