
#include "bootpack.h" 
#include "bootinfo.h"
#include "graphic.h"
#include "misc.h"
#include "mouse.h"

void HogeMain(void)
{
    const BootInfo* binfo = get_bootinfo();
    int xsize = binfo->scrnx;
    int ysize = binfo->scrny;
    init_pallet();

    boxfill8( 0, 0, 320, 200, COLOR_BLACK );

    boxfill8( 0,         0,          xsize -  1, ysize - 29, COLOR_DEEP_BLUE );
    boxfill8( 0,         ysize - 28, xsize -  1, ysize - 28, COLOR_LIGHT_GRAY );
    boxfill8( 0,         ysize - 27, xsize -  1, ysize - 27, COLOR_WHITE );
    boxfill8( 0,         ysize - 26, xsize -  1, ysize -  1, COLOR_LIGHT_GRAY );

    boxfill8( 3,         ysize - 24, 59,         ysize - 24, COLOR_WHITE );
    boxfill8( 2,         ysize - 24,  2,         ysize -  4, COLOR_WHITE );
    boxfill8( 3,         ysize -  4, 59,         ysize -  4, COLOR_DEEP_BLUE );
    boxfill8( 59,        ysize - 23, 59,         ysize -  5, COLOR_DEEP_BLUE );
    boxfill8( 2,         ysize -  3, 59,         ysize -  3, COLOR_BLACK );
    boxfill8( 60,        ysize - 24, 60,         ysize -  3, COLOR_BLACK );

    boxfill8( xsize - 47, ysize - 24, xsize -  4, ysize - 24, COLOR_DEEP_BLUE );
    boxfill8( xsize - 47, ysize - 23, xsize - 47, ysize -  4, COLOR_DEEP_BLUE );
    boxfill8( xsize - 47, ysize -  3, xsize -  4, ysize -  3, COLOR_WHITE );
    boxfill8( xsize -  3, ysize - 24, xsize -  3, ysize -  3, COLOR_WHITE );

    Point p;
    p = putstr8( 8, 8, COLOR_WHITE, "HOGEHOGE\n" );
    p = putstr8( p.x, p.y, COLOR_WHITE, dec2str(-2147483647) );

    uint8_t cursor[16*16];
    get_mouse_cursor_bitmap( cursor, COLOR_DEEP_BLUE );
    draw_cursor( 160, 100, cursor );

    while(1){
        _io_hlt();
    }
}

