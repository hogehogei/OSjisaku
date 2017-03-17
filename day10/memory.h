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

// @brief メモリ管理初期化
// @param begin_addr に開始アドレス、end_addr に終了アドレスを渡す
// @return  1 初期化成功   0 初期化失敗
int mem_allocator_init( uint32_t begin_addr, uint32_t end_addr );

// @brief メモリ確保
// @return  メモリの先頭アドレス　確保失敗の場合はNULL
void* mem_alloc( uint32_t size );

// @brief メモリ解放
// ptr == null の時は何もしないことを保証する
void mem_free( void* ptr );

// 空きメモリの総量を返す
uint32_t mem_free_total(void);

// @brief メモリ容量チェック   
// @param start 範囲チェック開始アドレス
// @param end   範囲チェック終了アドレス 
// @return  start <= end の範囲内でアクセスできるメモリの最大アドレス
uint32_t memtest( uint32_t start, uint32_t end );

#endif
