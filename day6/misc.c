
#include "misc.h"

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
