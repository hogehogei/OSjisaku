#ifndef    MOUSE_H_INCLUDED
#define    MOUSE_H_INCLUDED

#include "type.h"

typedef struct _MouseDecode
{
    uint8_t buf[3];
    uint8_t phase;
    int32_t x, y;
    uint32_t btn;
} MouseDecode;

#define    MOUSE_BTN_LEFT    0x01
#define    MOUSE_BTN_RIGHT   0x02
#define    MOUSE_BTN_CENTER  0x04

void enalbe_mouse(void);
void create_mouse_cursor_bitmap( uint8_t* cursor_dst, uint8_t bg_color );

// @brief I/Oポートから取得した生データをマウスの情報に変換
//        マウスの移動/ボタン情報は3バイトからなるので、複数回呼ぶ必要がある
// @return 0  まだデータが足りず変換が完了していない
// @return 1  マウスの情報を正しく変換できた。変換情報は MouseDecode内にある
// @return -1 エラー ありえないデータが渡された、データが壊れていた場合など
int mouse_decode( MouseDecode* mdec, uint8_t raw_data );

#endif
