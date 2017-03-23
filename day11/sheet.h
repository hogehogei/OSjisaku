#ifndef    SHEET_H_INCLUDED
#define    SHEET_H_INCLUDED

#include "type.h"

#define    MAX_SHEETS    256
#define    SHEET_UNUSED    0
#define    SHEET_INUSE     1

struct _SheetCtl;
typedef struct _Sheet {
    uint8_t* bitmap;       // vramに書くもの 
    int bxsize, bysize;    // 下敷きの X, Yサイズ
    int vx0, vy0;          // 画面上のどこにあるか
    int col_inv;           // 透明色
    int height;            // 下敷きの高さマップ
    int flag;
    struct _SheetCtl* ctl;         // これを管理するSheetCtlへのポインタ
    uint8_t sid;           // sheet ID
} Sheet;

typedef struct _SheetCtl {
    uint8_t* vram;
    uint8_t* map;        // vram描写時のsheetmap これを見て対応するsheetだけVRAMに書く
    int xsize, ysize;    // vramの X, Yサイズ
    int top;             // いちばん上の下敷きの高さ
    Sheet* sheets[MAX_SHEETS];     // Sheetを高さ順に並べ替えた後
    Sheet sheets0[MAX_SHEETS];     // Sheetの実体
} SheetCtl;

SheetCtl* sheetctl_init(void);
Sheet* sheet_alloc( SheetCtl* ctl );
void sheet_free( Sheet* sht );
void sheet_setbuf( Sheet* sht, uint8_t* bitmap, int xsize, int ysize, int col_inv );

void sheet_refreshmap( SheetCtl* ctl, int vx0, int vy0, int vx1, int vy1, int h0 );
void sheet_refreshall( SheetCtl* ctl );
void sheet_refresh( Sheet* sht );
void sheet_refreshsub( SheetCtl* ctl, int vx0, int vy0, int vx1, int vy1, int h0, int h1 );

void sheet_slide( Sheet* sht, int vx0, int vy0 );
void sheet_updown( Sheet* sht, int height );

#endif
