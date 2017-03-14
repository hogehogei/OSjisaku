// グローバル変数の実体化はこのファイルで
#define  GLOBAL_VALUE

#include "bootpack.h" 
#include "bootinfo.h"
#include "graphic.h"
#include "misc.h"
#include "mouse.h"
#include "interrupt.h"
#include "memory.h"
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
    int mx = 0, my = 0;
    MouseDecode mdec;
    enable_mouse( &mdec );
    create_mouse_cursor_bitmap( cursor, COLOR_DEEP_BLUE );
    draw_cursor( mx, my, cursor );

    _io_out8( PIC0_IMR, 0xF9 );    // PIC1とキーボードを許可 11111001
    _io_out8( PIC1_IMR, 0xEF );    // マウスを許可 11101111

    // memoryサイズ KB単位
    uint32_t memory_total = memtest( 0x00400000, 0xBFFFFFFF );
    init_mem_allocator( 0x00400000, memory_total );
    void* ptr = mem_alloc( 4000 );
    uint32_t freemem = mem_free_total();

    uint8_t key = 0;
    while(1){
        char s[4] = { 'l', 'c', 'r' };
        Point p = { 0, 0 };
        p = putstr8( p.x, p.y, COLOR_WHITE, "memory size : " );
        p = putstr8( p.x, p.y, COLOR_WHITE, dec2str(memory_total / 1024) );
        p = putstr8( 0, p.y, COLOR_WHITE, "\nmemory free : " );
        p = putstr8( p.x, p.y, COLOR_WHITE, dec2str(freemem / 1024) );
        p = putstr8( 0, p.y, COLOR_WHITE, "\nmem addr : " );
        p = putstr8( p.x, p.y, COLOR_WHITE, dec2str((uint32_t)ptr) );
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
                 if( mouse_decode( &mdec, key ) ){
                     if( mdec.btn & MOUSE_BTN_LEFT ){
                         s[0] = 'L';
                     }
                     if( mdec.btn & MOUSE_BTN_CENTER ){
                         s[1] = 'C';
                     }
                     if( mdec.btn & MOUSE_BTN_RIGHT ){
                         s[2] = 'R';
                     }
                     boxfill8( 0, 0, 24, 15, COLOR_DEEP_BLUE );
                     putstr8( 0, 0, COLOR_WHITE, s );

                     boxfill8( mx, my, mx+15, my+15, COLOR_DEEP_BLUE );
                     mx += mdec.x;
                     my += mdec.y;
                     if( mx < 0 ){
                         mx = 0;
                     }
                     if( my < 0 ){
                         my = 0;
                     }
                     if( mx > binfo->scrnx ){
                         mx = binfo->scrnx;
                     }
                     if( my > binfo->scrny ){
                         my = binfo->scrny;
                     }

                     draw_cursor( mx, my, cursor );
                 }
             }
       }
    }
}
