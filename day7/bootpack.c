// グローバル変数の実体化はこのファイルで
#define  GLOBAL_VALUE

#include "bootpack.h" 
#include "bootinfo.h"
#include "graphic.h"
#include "misc.h"
#include "mouse.h"
#include "interrupt.h"
#include "global.h"

void HogeMain(void)
{
    const BootInfo* binfo = get_bootinfo();
    int xsize = binfo->scrnx;
    int ysize = binfo->scrny;

    init_gdtidt();
    init_pic();
    _io_sti();    // IDT/PICの初期化が終わったのでCPUの割り込み禁止を解除

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


    // キーボード初期化
    init_keyboard();

    // マウス初期化 
    uint8_t cursor[16*16];
    init_mouse();
    create_mouse_cursor_bitmap( cursor, COLOR_DEEP_BLUE );
    draw_cursor( 160, 100, cursor );

    _io_out8( PIC0_IMR, 0xF9 );    // PIC1とキーボードを許可 11111001
    _io_out8( PIC1_IMR, 0xEF );    // マウスを許可 11101111

    uint8_t key = 0;
    while(1){
        _io_cli();
        if( fifo8_datasize(&gKeyFIFO) == 0 && fifo8_datasize(&gMouseFIFO) == 0 ){
            _io_stihlt();
        }
        else {
            // バッファ内にデータがある
            if( fifo8_datasize(&gKeyFIFO) ){
                fifo8_get( &gKeyFIFO, &key );
                _io_sti();
                boxfill8( 0, 0, 31, 15, COLOR_DEEP_BLUE );
                putstr8( 0, 0, COLOR_WHITE, dec2str(key) );
             }
             else {
                 fifo8_get( &gMouseFIFO, &key );
                 _io_sti();
                 boxfill8( 0, 16, 31, 32, COLOR_DEEP_BLUE );
                 putstr8( 0, 16, COLOR_RED, dec2str(key) );
             }
       }
        //putstr8( 0, 16, COLOR_WHITE, "HOGEHOGE" );
    }
}
