#ifndef    GLOBAL_H_INCLUDED
#define    GLOBAL_H_INCLUDED

#ifdef    GLOBAL_VALUE
  #define   GLOBAL
#else
  #define   GLOBAL  extern
#endif

GLOBAL FIFO8 gKeyFIFO;
GLOBAL FIFO8 gMouseFIFO;

#endif
