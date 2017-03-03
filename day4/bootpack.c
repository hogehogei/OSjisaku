
#include "bootpack.h" 

void init_pallet(void);
void set_pallet( int start, int end, unsigned char* rgb );
void boxfill8( const BootInfo* binfo, 
               unsigned char color,
               int x0, int y0, 
               int x1, int y1 );


void HogeMain(void)
{
    int i = 0;

    const BootInfo* binfo = (const BootInfo*)0x0FF0;
    int xsize = binfo->scrnx;
    int ysize = binfo->scrny;
    init_pallet();

    boxfill8( binfo, COLOR_BLACK, 0, 0, 320, 200 );
    //boxfill8( binfo, 1, 20, 20, 120, 120 );
    //boxfill8( binfo, 2, 70, 50, 170, 150 );
    //boxfill8( binfo, 4, 120, 80, 220, 180 );

    boxfill8( binfo, COLOR_DEEP_BLUE,  0,         0,          xsize -  1, ysize - 29);
    boxfill8( binfo, COLOR_LIGHT_GRAY,  0,         ysize - 28, xsize -  1, ysize - 28);
    boxfill8( binfo, COLOR_WHITE,  0,         ysize - 27, xsize -  1, ysize - 27);
    boxfill8( binfo, COLOR_LIGHT_GRAY,  0,         ysize - 26, xsize -  1, ysize -  1);

    boxfill8( binfo, COLOR_WHITE, 3,         ysize - 24, 59,         ysize - 24);
    boxfill8( binfo, COLOR_WHITE, 2,         ysize - 24,  2,         ysize -  4);
    boxfill8( binfo, COLOR_DEEP_BLUE,  3,         ysize -  4, 59,         ysize -  4);
    boxfill8( binfo, COLOR_DEEP_BLUE, 59,         ysize - 23, 59,         ysize -  5);
    boxfill8( binfo, COLOR_BLACK,  2,         ysize -  3, 59,         ysize -  3);
    boxfill8( binfo, COLOR_BLACK, 60,         ysize - 24, 60,         ysize -  3);

    boxfill8( binfo, COLOR_DEEP_BLUE, xsize - 47, ysize - 24, xsize -  4, ysize - 24);
    boxfill8( binfo, COLOR_DEEP_BLUE, xsize - 47, ysize - 23, xsize - 47, ysize -  4);
    boxfill8( binfo, COLOR_WHITE, xsize - 47, ysize -  3, xsize -  4, ysize -  3);
    boxfill8( binfo, COLOR_WHITE, xsize -  3, ysize - 24, xsize -  3, ysize -  3);


    while(1){
        _io_hlt();
    }
}

void boxfill8( const BootInfo* binfo, 
               unsigned char color,
               int x0, int y0, 
               int x1, int y1 )
{
    int x, y;
    int scrnx = binfo->scrnx;
    int scrny = binfo->scrny;
    char* vram = binfo->vram;

    for( y = y0; y <= y1 && y < scrny; ++y ){
        for( x = x0; x <= x1 && x < scrnx; ++x ){
            if( x >= 0 && y >= 0 ){
                vram[y * scrnx + x] = color;
            }
        }
    }
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

    int* mark = 0x1002;
    *mark = skTableRGB;

    set_pallet( 0, 15, skTableRGB );
}

void set_pallet( int start, int end, unsigned char* rgb )
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
