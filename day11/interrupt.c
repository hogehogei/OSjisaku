
#include "interrupt.h"
#include "bootpack.h"
#include "graphic.h"
#include "misc.h"
#include "global.h"

void init_pic(void)
{
    _io_out8( PIC0_IMR, 0xFF );    // すべての割り込みを受け付けない
    _io_out8( PIC1_IMR, 0xFF );    // すべての割り込みを受け付けない

    _io_out8( PIC0_ICW1, 0x11 );   // エッジトリガモード
    _io_out8( PIC0_ICW2, 0x20 );   // IRQ0-7 は INT20-27で受ける
    _io_out8( PIC0_ICW3, 1 << 2 ); // PIC1はIRQ2にて接続
    _io_out8( PIC0_ICW4, 0x01 );   // ノンバッファモード
    
    _io_out8( PIC1_ICW1, 0x11 );   // エッジトリガモード
    _io_out8( PIC1_ICW2, 0x28 );   // IRQ8-15 は INT28-2Fで受ける
    _io_out8( PIC1_ICW3, 2 );      // PIC1はIRQ2にて接続
    _io_out8( PIC1_ICW4, 0x01 );   // ノンバッファモード

    _io_out8( PIC0_IMR, 0xFB );    // 11111011 PIC1以外はすべて禁止
    _io_out8( PIC1_IMR, 0xFF );    // 11111111 すべての割り込みを受け付けない

    return;
}

void _inthandler21_keyboard( int* esp )
{
    uint8_t data;
    _io_out8( PIC0_OCW2, 0x61 );    // IRQ-01 受付完了をPICに通知
    data = _io_in8( PORT_KEYDAT );
    fifo8_put( &gKeyFIFO, data );
}

void _inthandler2c_mouse( int* esp )
{
    uint8_t data;
    _io_out8( PIC1_OCW2, 0x64 );    // IRQ-12 受付完了をPIC1に通知
    _io_out8( PIC0_OCW2, 0x62 );    // IRQ-02 受付完了をPIC0に通知
    data = _io_in8( PORT_KEYDAT );
    fifo8_put( &gMouseFIFO, data );
}
