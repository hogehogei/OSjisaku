#include "window.h"
#include "sheet.h"
#include "graphic.h"


void make_window8( Sheet* sht, const char* title )
{
    static char closebtn[14][16] = {
        "OOOOOOOOOOOOOOO@",
        "OQQQQQQQQQQQQQ$@",
        "OQQQQQQQQQQQQQ$@",
        "OQQQ@@QQQQ@@QQ$@",
        "OQQQQ@@QQ@@QQQ$@",
        "OQQQQQ@@@@QQQQ$@",
        "OQQQQQQ@@QQQQQ$@",
        "OQQQQQ@@@@QQQQ$@",
        "OQQQQ@@QQ@@QQQ$@",
        "OQQQ@@QQQQ@@QQ$@",
        "OQQQQQQQQQQQQQ$@",
        "OQQQQQQQQQQQQQ$@",
        "O$$$$$$$$$$$$$$@",
        "@@@@@@@@@@@@@@@@"
    };

    int xsize = sht->bxsize;
    int ysize = sht->bysize;
    boxfill8( sht, 0,         0,         xsize - 1, 0, COLOR_LIGHT_GRAY );
    boxfill8( sht, 1,         1,         xsize - 2, 1, COLOR_WHITE );
    boxfill8( sht, 0,         0,         0,         ysize - 1, COLOR_LIGHT_GRAY );
    boxfill8( sht, 1,         1,         1,         ysize - 2, COLOR_WHITE );
    boxfill8( sht, xsize - 2, 1,         xsize - 2, ysize - 2, COLOR_DARK_GRAY );
    boxfill8( sht, xsize - 1, 0,         xsize - 1, ysize - 1, COLOR_BLACK );
    boxfill8( sht, 2,         2,         xsize - 3, ysize - 3, COLOR_LIGHT_GRAY );
    boxfill8( sht, 3,         3,         xsize - 4, 20, COLOR_DARK_BLUE );
    boxfill8( sht, 1,         ysize - 2, xsize - 2, ysize - 2, COLOR_DARK_GRAY );
    boxfill8( sht, 0,         ysize - 1, xsize - 1, ysize - 1, COLOR_BLACK );
    putstr8( sht, 24, 4, COLOR_WHITE, COLOR_DARK_BLUE, title );

    int x, y;
    char c;
    uint8_t* bmap = sht->bitmap;
    for (y = 0; y < 14; y++) {
        for (x = 0; x < 16; x++) {
            c = closebtn[y][x];
            if (c == '@') {
                c = COLOR_BLACK;
            } else if (c == '$') {
                c = COLOR_DARK_GRAY;
            } else if (c == 'Q') {
                c = COLOR_LIGHT_GRAY;
            } else {
                c = COLOR_WHITE;
            }
            bmap[(5 + y) * xsize + (xsize - 21 + x)] = c;
        }
    }
}
