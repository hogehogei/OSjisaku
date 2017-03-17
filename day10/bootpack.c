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
    
    // キーボード初期化
    init_keyboard();

    // マウス初期化 
    uint8_t cursor_bmap[16*16];
    int mx = 0, my = 0;
    MouseDecode mdec;
    enable_mouse( &mdec );
    create_mouse_cursor_bitmap( cursor_bmap, COLOR_INVISIBLE );

    _io_out8( PIC0_IMR, 0xF9 );    // PIC1とキーボードを許可 11111001
    _io_out8( PIC1_IMR, 0xEF );    // マウスを許可 11101111

    // メモリアロケータ初期化
    uint32_t memory_total = memtest( 0x00400000, 0xBFFFFFFF );
    init_mem_allocator( 0x00400000, memory_total );

    // シートを作成
    SheetCtl* ctl;
    Sheet* sht_back;
    Sheet* sht_mouse;
    uint8_t* back_bmap = (uint8_t*)mem_alloc( xsize * ysize );

    ctl = sheetctl_init();
    sht_back = sheet_alloc( ctl );
    sht_mouse = sheet_alloc( ctl );
    sheet_setbuf( sht_back, back_bmap, xsize, ysize, COLOR_INVISIBLE );
    sheet_setbuf( sht_mouse, cursor_bmap, 16, 16, COLOR_INVISIBLE );
    sheet_updown( sht_back, 0 );
    sheet_updown( sht_mouse, 1 );

    // 画面初期化
    init_screen( sht_back );
    sheet_refresh( ctl );

    uint32_t freemem = mem_free_total();
    uint8_t key = 0;
    while(1){
        char s[4] = { 'l', 'c', 'r' };
        Point p = { 0, 0 };
        p = putstr8( sht_back, 0, 0, COLOR_WHITE, "memory size : " );
        p = putstr8( sht_back, p.x, p.y, COLOR_WHITE, dec2str(memory_total / 1024) );
        p = putstr8( sht_back, 0, p.y, COLOR_WHITE, "\nmemory free : " );
        p = putstr8( sht_back, p.x, p.y, COLOR_WHITE, dec2str(freemem / 1024) );
        _io_cli();
        if( fifo8_datasize(&gKeyFIFO) == 0 && fifo8_datasize(&gMouseFIFO) == 0 ){
            _io_stihlt();
        }
        else {
            // バッファ内にデータがある
            if( fifo8_datasize(&gKeyFIFO) ){
                fifo8_get( &gKeyFIFO, &key );
                _io_sti();
                boxfill8( sht_back, 0, 0, 31, 15, COLOR_DEEP_BLUE );
                putstr8( sht_back, 0, 0, COLOR_WHITE, dec2str(key) );
                sheet_refresh( ctl );
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
                     boxfill8( sht_back, 0, 0, 24, 15, COLOR_DEEP_BLUE );
                     putstr8( sht_back, 0, 0, COLOR_WHITE, s );

                     mx += mdec.x;
                     my += mdec.y;
                     if( mx < 0 ){
                         mx = 0;
                     }
                     if( my < 0 ){
                         my = 0;
                     }
                     if( mx > binfo->scrnx ){
                         mx = binfo->scrnx - 1;
                     }
                     if( my > binfo->scrny ){
                         my = binfo->scrny - 1 ;
                     }

                     sheet_slide( sht_mouse, mx, my );
                 }
             }
       }
    }
}
