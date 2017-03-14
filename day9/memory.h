#ifndef    MEMORY_H_INCLUDED
#define    MEMORY_H_INCLUDED

#include "type.h"

#define    EFLAGS_AC_BIT      0x00040000
#define    CR0_CACHE_DISABLE  0x60000000

typedef struct _MemHeader {
    uint32_t size;
    uint32_t used;
    struct _MemHeader* prev;
    struct _MemHeader* next;
} MemHeader;

typedef struct _MemEnd {
    uint32_t pattern;
    uint32_t size;
} MemEnd;

#define    MEMORY_LINK_OVERHEAD    (sizeof(MemHeader*) * 2)
#define    MEMORY_HDR_OVERHEAD     (sizeof(MemHeader)+sizeof(MemEnd))
#define    MEMORY_MIN_ALLOC        8
#define    MEMORY_FREELIST_SIZE    256
#define    MEMORY_CHECK_PATTERN    0xAA55AA55

typedef struct _MemManager {
    MemHeader* begin_addr;
    uint32_t mem_total;
} MemManager;

int mem_allocator_init( uint32_t begin_addr, uint32_t end_addr );
void* mem_alloc( uint32_t size );
void mem_free( void* ptr );
uint32_t mem_free_total(void);

// @brief メモリ容量チェック   
// @param start 範囲チェック開始アドレス
// @param end   範囲チェック終了アドレス 
// @return  start <= end の範囲内でアクセスできるメモリの最大アドレス
uint32_t memtest( uint32_t start, uint32_t end );

#endif
