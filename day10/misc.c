
#include "misc.h"

void fifo8_init( FIFO8* fifo, uint8_t* buf, int bufsize )
{
    fifo->buf  = buf;
    fifo->size = bufsize;
    fifo->free = bufsize;
    fifo->ridx = 0;
    fifo->widx = 0;
    fifo->flag = 0;
}

int fifo8_put( FIFO8* fifo, uint8_t data )
{
    int wnext = (fifo->widx + 1) % fifo->size;

    // バッファオーバーランするので読み捨てる
    if( wnext == fifo->ridx ){
        fifo->flag |= FIFO8_OVERRUN;
        return 0;
    }

    fifo->buf[fifo->widx] = data;
    fifo->widx = wnext;
    --(fifo->free);

    return 1;
}

int fifo8_get( FIFO8* fifo, uint8_t* dst )
{
    int rnext = 0;
    // バッファは空
    if( fifo->ridx == fifo->widx ){
        return 0;
    }

    *dst = fifo->buf[fifo->ridx];
    // 読みインデックスを進める
    rnext = (fifo->ridx + 1) % fifo->size;
    fifo->ridx = rnext;
    ++(fifo->free);

    return 1;
}

int fifo8_datasize( const FIFO8* fifo )
{
    return fifo->size - fifo->free;
}


const char* dec2str( int n )
{
    static char buf[12];    // -2147483648 から 2147483647 の範囲なのでNULL含めて12けた
    char* p = buf + 11;
    int t = n;
    int minus = 0;

    *p = 0;
    if( t < 0 ){
        minus = 1;
        t *= -1;
    }
    else if( t == 0 ){
        *(--p) = '0';
        return p; 
    }

    
    do {
        *(--p) = '0' + (t % 10);
        t /= 10;
    } while( t > 0 );

    if( minus ){
        *(--p) = '-';
    }

    return p;
}
