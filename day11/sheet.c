#include "sheet.h"
#include "bootinfo.h"

SheetCtl* sheetctl_init(void)
{
    SheetCtl* ctl = (SheetCtl*)mem_alloc( sizeof(SheetCtl) );
    if( !ctl ){
        goto sheetctl_init_error;
    }

    const BootInfo* binfo = get_bootinfo();
    int i = 0;

    ctl->vram = binfo->vram;
    ctl->xsize = binfo->scrnx;
    ctl->ysize = binfo->scrny;

    ctl->map = (uint8_t*)mem_alloc( ctl->xsize * ctl->ysize );
    if( !ctl->map ){
        goto sheetctl_init_error;
    }

    ctl->top = -1;    // シートは1枚もない
    for( i = 0; i < MAX_SHEETS; ++i ){
        ctl->sheets0[i].flag = SHEET_UNUSED;    // 未使用マーク
    }

    // 正常終了
    return ctl;

sheetctl_init_error:
    mem_free( ctl );
    return 0;
}

Sheet* sheet_alloc( SheetCtl* ctl )
{
    Sheet* sht = 0;
    int i = 0;
    
    for( i = 0; i < MAX_SHEETS; ++i ){
        if( ctl->sheets0[i].flag == SHEET_UNUSED ){
            sht = &(ctl->sheets0[i]);
            sht->flag = SHEET_INUSE;
            sht->height = -1;    // 非表示中
            sht->ctl = ctl;
            sht->sid = i;        // sheet id 配列の何番目かを入れておく
            return sht;
        }
    }

    // すべて使用中で使用できるシートがない
    return 0;
}

void sheet_free( Sheet* sht )
{
    if( sht->height >= 0 ){
        sheet_updown( sht, -1 );    // 表示中ならまず非表示にする
    }
    sht->flag = SHEET_UNUSED;    // 未使用マーク
}

void sheet_setbuf( Sheet* sht, uint8_t* bitmap, int xsize, int ysize, int col_inv )
{
    sht->bitmap = bitmap;
    sht->bxsize = xsize;
    sht->bysize = ysize;
    sht->col_inv = col_inv;
}

void sheet_refreshmap( SheetCtl* ctl, int vx0, int vy0, int vx1, int vy1, int h0 )
{
    int h = 0, bx, by, vx, vy;
    int bx0, by0, bx1, by1;
    uint8_t* vram = ctl->vram;
    uint8_t* map = ctl->map;
    uint8_t* bmap = 0;
    Sheet* sht = 0;

    // map画面外なら補正する
    if( vx0 < 0 ) { vx0 = 0; }
    if( vy0 < 0 ) { vy0 = 0; }
    if( vx1 > ctl->xsize ) { vx1 = ctl->xsize; }
    if( vy1 > ctl->ysize ) { vy1 = ctl->ysize; }

    for( h = h0; h <= ctl->top; ++h ){
        sht = ctl->sheets[h];
        bmap = sht->bitmap;
        
        // 対象のsheetの左上を(0, 0)にした場合の座標系に変換
        // vx0 = mapへの転送座標、 sht->vx0 = sheetの左上が map上のどこにあるか
        // bx0 = sheet 上のX座標
        // vx0 = sht->vx0 + bx0 を式変換
        bx0 = vx0 - sht->vx0;
        by0 = vy0 - sht->vy0;
        bx1 = vx1 - sht->vx0;
        by1 = vy1 - sht->vy0;
        // 更新範囲をsheet範囲内になるようにクリップ
        if( bx0 < 0 ){ bx0 = 0; }
        if( by0 < 0 ){ by0 = 0; }
        if( bx1 >= sht->bxsize ){ bx1 = sht->bxsize; }
        if( by1 >= sht->bysize ){ by1 = sht->bysize; }

        for( by = by0; by < by1; ++by ){
            vy = sht->vy0 + by;
            for( bx = bx0; bx < bx1; ++bx ){
                vx = sht->vx0 + bx;
                if( bmap[by * sht->bxsize + bx] != sht->col_inv ){
                    map[vy * ctl->xsize + vx] = sht->sid;
                }
            }
        }
    }
}

void sheet_refreshall( SheetCtl* ctl )
{
    int h = 0, bx, by, vx, vy;
    uint8_t* vram = ctl->vram;
    uint8_t* bmap = 0;
    uint8_t c = 0;
    Sheet* sht = 0;

    for( h = 0; h <= ctl->top; ++h ){
        sht = ctl->sheets[h];
        bmap = sht->bitmap;
        for( by = 0, vy = sht->vy0; 
             by < sht->bysize && vy < ctl->ysize; 
             ++by, ++vy ){

            for( bx = 0, vx = sht->vx0; 
                 bx < sht->bxsize && vx < ctl->xsize; 
                 ++bx, ++vx ){

                c = bmap[by * sht->bxsize + bx];
                if( c != sht->col_inv && vx >= 0 && vy >= 0 ){
                    vram[vy * ctl->xsize + vx] = c;
                }
            }
        }
    }
}

void sheet_refresh( Sheet* sht )
{
    if( sht->height >= 0 ){
        sheet_refreshsub( sht->ctl,
                          sht->vx0,
                          sht->vy0,
                          sht->vx0 + sht->bxsize,
                          sht->vy0 + sht->bysize, 
                          sht->height,
                          sht->height );
    }
}

void sheet_refreshsub( SheetCtl* ctl, int vx0, int vy0, int vx1, int vy1, int h0, int h1 )
{
    int h = 0, bx, by, vx, vy;
    int bx0, by0, bx1, by1;
    uint8_t* vram = ctl->vram;
    uint8_t* map = ctl->map;
    uint8_t* bmap = 0;
    uint8_t c = 0;
    Sheet* sht = 0;

    // VRAM画面外なら補正する
    if( vx0 < 0 ) { vx0 = 0; }
    if( vy0 < 0 ) { vy0 = 0; }
    if( vx1 > ctl->xsize ) { vx1 = ctl->xsize; }
    if( vy1 > ctl->ysize ) { vy1 = ctl->ysize; }

    for( h = h0; h <= h1; ++h ){
        sht = ctl->sheets[h];
        bmap = sht->bitmap;
        
        // 対象のsheetの左上を(0, 0)にした場合の座標系に変換
        // vx0 = VRAMへの転送座標、 sht->vx0 = sheetの左上が vram上のどこにあるか
        // bx0 = sheet 上のX座標
        // vx0 = sht->vx0 + bx0 を式変換
        bx0 = vx0 - sht->vx0;
        by0 = vy0 - sht->vy0;
        bx1 = vx1 - sht->vx0;
        by1 = vy1 - sht->vy0;
        // 更新範囲をsheet範囲内になるようにクリップ
        if( bx0 < 0 ){ bx0 = 0; }
        if( by0 < 0 ){ by0 = 0; }
        if( bx1 >= sht->bxsize ){ bx1 = sht->bxsize; }
        if( by1 >= sht->bysize ){ by1 = sht->bysize; }

        for( by = by0; by < by1; ++by ){
            vy = sht->vy0 + by;
            for( bx = bx0; bx < bx1; ++bx ){
                vx = sht->vx0 + bx;
                if( map[vy * ctl->xsize + vx] == sht->sid ){
                    vram[vy * ctl->xsize + vx] = bmap[by * sht->bxsize + bx];
                }
            }
        }
    }
}

void sheet_slide( Sheet* sht, int vx0, int vy0 )
{
    int old_vx0 = sht->vx0;
    int old_vy0 = sht->vy0;
    sht->vx0 = vx0;
    sht->vy0 = vy0;

    if( sht->height >= 0 ){
        // 移動前の範囲を更新
        sheet_refreshmap( sht->ctl, old_vx0, old_vy0, old_vx0 + sht->bxsize, old_vy0 + sht->bysize, 0 );
        sheet_refreshsub( sht->ctl, old_vx0, old_vy0, old_vx0 + sht->bxsize, old_vy0 + sht->bysize, 0, sht->height - 1 );
        // 移動後の範囲を更新
        sheet_refreshmap( sht->ctl, vx0, vy0, vx0 + sht->bxsize, vy0 + sht->bysize, sht->height );
        sheet_refreshsub( sht->ctl, vx0, vy0, vx0 + sht->bxsize, vy0 + sht->bysize, sht->height, sht->height );
    }
}

void sheet_updown( Sheet* sht, int height )
{
    SheetCtl* ctl = sht->ctl;
    int old_h = sht->height;

    if( height > ctl->top + 1 ){
        height = ctl->top + 1;
    }
    if( height < -1 ){
        height = -1;
    }

    sht->height = height;

    int h = 0;
    if( old_h < height ){
        if( old_h >= 0 ){
            // 間のものを押し下げる
            for( h = old_h; h < height; ++h ){
                ctl->sheets[h] = ctl->sheets[h+1];
                ctl->sheets[h]->height = h;
            }
            ctl->sheets[height] = sht;

        }
        else {    // 非表示状態から表示状態へ
            for( h = ctl->top; h >= height; --h ){
                ctl->sheets[h+1] = ctl->sheets[h];
                ctl->sheets[h+1]->height = h+1;
            }
            ctl->sheets[height] = sht;
            ++(ctl->top);    // 表示中の下敷きが1増えるので、いちばん上の高さも1増える
        }
        sheet_refreshmap( sht->ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize, height );
        sheet_refreshsub( sht->ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize, height, height );
 
   }
    else if( old_h > height ){
        if( old_h >= 0 ){
            // 間のものを押し上げる
            for( h = old_h; h < height; --h ){
                ctl->sheets[h] = ctl->sheets[h-1];
                ctl->sheets[h]->height = h;
            }
            ctl->sheets[height] = sht;
            sheet_refreshmap( sht->ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize, height+1 );
            sheet_refreshsub( sht->ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize, height+1, old_h );
         }
        else {    // 表示状態から非表示に
            // 一番上の下敷き以外なら上になっているものを下ろす
            if( ctl->top > old_h ){   
                for( h = old_h; h < ctl->top; ++h ){
                    ctl->sheets[h] = ctl->sheets[h+1];
                    ctl->sheets[h]->height = h;
                }
            }
            --(ctl->top);    //　表示中の下敷きが1つ減るので、いちばん上の高さも1つ減る
            sheet_refreshmap( sht->ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize, 0 );
            sheet_refreshsub( sht->ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize, 0, old_h - 1 );
         }

   }
}
