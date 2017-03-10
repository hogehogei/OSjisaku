#ifndef    MOUSE_H_INCLUDED
#define    MOUSE_H_INCLUDED

#include "type.h"

void init_mouse(void);
void create_mouse_cursor_bitmap( uint8_t* cursor_dst, uint8_t bg_color );

#endif
