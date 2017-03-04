
#include "interrupt.h"
#include "bootpack.h"
#include "graphic.h"

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
    putstr8( 0, 0, COLOR_WHITE, "INT 21 (IRQ-1) : PS/2 keyboard" );
    while(1){
        _io_hlt();
    }
}

void _inthandler2c_mouse( int* esp )
{
    putstr8( 0, 0, COLOR_WHITE, "INT 2C (IRQ-12) : PS/2 mouse" );
    while(1){
        _io_hlt();
    }
}

