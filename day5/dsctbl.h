
#ifndef    DSCTBL_H_INCLUDED
#define    DSCTBL_H_INCLUDED

#include "type.h"

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

