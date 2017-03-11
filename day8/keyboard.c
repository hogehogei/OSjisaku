#include "keyboard.h"
#include "misc.h"
#include "global.h"

void wait_KBC_sendready(void)
{
    // キーボードコントローラがデータ送信可能になるのを待つ
    while(1){
        if( (_io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0 ){
            break;
        }
    }
}

void init_keyboard(void)
{
    static uint8_t keybuf[32];

    wait_KBC_sendready();
    _io_out8( PORT_KEYCMD, KEYCMD_WRITE_MODE );
    wait_KBC_sendready();
    _io_out8( PORT_KEYDAT, KBC_MODE );

    // キーバッファの初期化
    fifo8_init( &gKeyFIFO, keybuf, sizeof(keybuf) );
}
