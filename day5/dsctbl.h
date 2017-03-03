
#ifndef    DSCTBL_H_INCLUDED
#define    DSCTBL_H_INCLUDED

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
void set_segmdesc( SegmentDescriptor* sd, unsigned int limit, int base, int ar );
void set_gatedesc( GateDescriptor* sd, int offset, int selector, int ar );
void loag_gdtr( int limit, int addr );
void load_idtr( int limit, int addr );

#endif

