#ifndef    DSCTBL_H_INCLUDED
#define    DSCTBL_H_INCLUDED

#include "type.h"

#define ADR_IDT      0x0026F800
#define LIMIT_IDT    0x000007FF
#define ADR_GDT      0x00270000
#define LIMIT_GDT    0x0000FFFF
#define ADR_BOTPAK   0x00280000
#define LIMIT_BOTPAK 0x0007FFFF
#define AR_DATA32_RW 0x4092    // システム専用の読み書き可能なセグメント。実行はできない
#define AR_CODE32_ER 0x409A    // システム専用の実行可能なセグメント。読めるけど書き込み不可  
#define AR_INTGATE32 0x008E


typedef struct _SegmentDescriptor {
    short limit_low, base_low;
    char base_mid, access_right;
    char limit_high, base_high;
} SegmentDescriptor;

typedef struct _GateDescriptor {
    short offset_low, selector;
    char dw_count, access_right;
    short offset_high;
} GateDescriptor;

void init_gdtidt(void);
void set_segmdesc( SegmentDescriptor* sd, uint32_t limit, uint32_t base, uint32_t ar );
void set_gatedesc( GateDescriptor* sd, uint32_t offset, uint32_t selector, uint32_t ar );
void _load_gdtr( int limit, int addr );
void _load_idtr( int limit, int addr );

#endif
