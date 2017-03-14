#ifndef    MISC_H_INCLUDED
#define    MISC_H_INCLUDED

#include "type.h"

typedef struct _Point {
    int x, y;
} Point;

typedef struct _FIFO8 {
    uint8_t* buf;
    int ridx, widx, size, free;
    uint32_t flag;
} FIFO8;

#define    FIFO8_OVERRUN    0x00000001

void fifo8_init( FIFO8* fifo, uint8_t* buf, int bufsize );
int fifo8_put( FIFO8* fifo, uint8_t data );
int fifo8_get( FIFO8* fifo, uint8_t* dst );
int fifo8_datasize( const FIFO8* fifo );

const char* dec2str( int n );

#endif
