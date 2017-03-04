#include "dsctbl.h"

void init_gdtidt(void)
{
    SegmentDescriptor* gdt = (SegmentDescriptor*)0x00270000;
    GateDescriptor* idt = (GateDescriptor*)0x0026F800;
    int i;

    // GDTの初期化
    for( i = 0; i < 8192; ++i ){
        set_segmdesc( gdt + i, 0, 0, 0 );
    }
    set_segmdesc( gdt+1, 0xFFFFFFFF, 0x00000000, 0x4092 );   // メモリ全体 4GB
    set_segmdesc( gdt+2, 0x0007FFFF, 0x00280000, 0x409A );   // 512KB bootpack用
    _load_gdtr( 0xFFFF, 0x00270000 );

    // IDTの初期化
    for( i = 0; i < 256; ++i ){
        set_gatedesc( idt+i, 0, 0, 0 );
    }
    _load_idtr( 0x7FF, 0x0026F800 );

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
