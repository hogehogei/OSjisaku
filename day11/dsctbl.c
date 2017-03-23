#include "dsctbl.h"
#include "interrupt.h"

void init_gdtidt(void)
{
    SegmentDescriptor* gdt = (SegmentDescriptor*)ADR_GDT;
    GateDescriptor* idt = (GateDescriptor*)ADR_IDT;
    int i;

    // GDTの初期化
    for( i = 0; i < 8192; ++i ){
        set_segmdesc( gdt + i, 0, 0, 0 );
    }
    set_segmdesc( gdt+1, 0xFFFFFFFF, 0x00000000,  AR_DATA32_RW );   // メモリ全体 4GB
    set_segmdesc( gdt+2, LIMIT_BOTPAK, ADR_BOTPAK, AR_CODE32_ER );   // 512KB bootpack用
    _load_gdtr( LIMIT_GDT, ADR_GDT );

    // IDTの初期化
    for( i = 0; i < 256; ++i ){
        set_gatedesc( idt+i, 0, 0, 0 );
    }
    _load_idtr( LIMIT_IDT, ADR_IDT );

    // IDTの設定 割り込みハンドラ登録
    set_gatedesc( idt+0x21, (int)_asm_inthandler21, 2 * 8, AR_INTGATE32 );
    set_gatedesc( idt+0x2c, (int)_asm_inthandler2c, 2 * 8, AR_INTGATE32 );

    return;
}

void set_segmdesc( SegmentDescriptor* sd, uint32_t limit, uint32_t base, uint32_t ar )
{
    // 20bit を超えるのでページ単位であると解釈
    if( limit > 0xFFFFF ){
        ar |= 0x8000;       // G_bit = 1
        limit /= 0x1000;    // 4Kで割る
    }

    sd->limit_low = limit & 0xFFFF;
    sd->base_low  = base & 0xFFFF;
    sd->base_mid  = (base >> 16) & 0xFF;
    sd->base_high = (base >> 24) & 0xFF;
    sd->access_right = ar & 0xFF;
    sd->limit_high   = ((limit >> 16) & 0x0F) | ((ar >> 8) & 0xF0);

    return;
}

void set_gatedesc( GateDescriptor* gd, uint32_t offset, uint32_t selector, uint32_t ar )
{
    gd->offset_low = offset & 0xFFFF;
    gd->selector   = selector;
    gd->dw_count   = (ar >> 8) & 0xFF;
    gd->access_right = ar & 0xFF;
    gd->offset_high  = (offset >> 16) & 0xFFFF;
}
