#include "memory.h"

static MemManager sMemMan;

static void init_memhdr( MemHeader* hdr, uint32_t size )
{
    hdr->size = size - MEMORY_HDR_OVERHEAD;
    hdr->used = 0;
    hdr->prev = 0;
    hdr->next = 0;

    uint8_t* p = (uint8_t*)hdr;
    p += size - sizeof(MemEnd);
    MemEnd* end = (MemEnd*)p;

    end->pattern = MEMORY_CHECK_PATTERN;
    end->size = hdr->size;
}

static MemHeader* mem_nexthdr( MemHeader* hdr )
{
    return hdr->next;
}

static MemHeader* mem_prevhdr( MemHeader* hdr )
{
    return hdr->prev;
}

static MemHeader* mem_split( MemHeader* hdr, uint32_t size )
{
    MemHeader *h1, *h2;
    MemEnd *h1e, *h2e;
    uint8_t* p = (uint8_t*)hdr;
    uint32_t org_size = hdr->size;
    
    h1 = hdr;
    h1->size = size;
    p += sizeof(MemHeader) + h1->size;
    h1e = (MemEnd*)p;
    h1e->size = size;
    h1e->pattern = MEMORY_CHECK_PATTERN;

    p += sizeof(MemEnd);
    h2 = (MemHeader*)p;
    init_memhdr( h2, org_size - size );

    MemHeader* h3 = h1->next;
    h1->next = h2;
    h2->prev = h1;
    h2->next = h3;
    h3->prev = h2;

    return hdr;
}

static MemHeader* mem_merge( MemHeader* l, MemHeader* r )
{
    uint32_t l_size = l->size;
    uint32_t r_size = r->size;
    MemHeader* rnext = r->next;

    init_memhdr( r, r_size + l_size + (MEMORY_HDR_OVERHEAD * 2) );
    l->next = rnext;
    rnext->prev = l;

    return l;
}

MemManager* get_memman(void)
{
    return &sMemMan;
}

int init_mem_allocator( uint32_t begin_addr, uint32_t end_addr )
{
    int memsize = end_addr - begin_addr;
    MemManager* memman = get_memman();

    if( begin_addr > end_addr || memsize < MEMORY_HDR_OVERHEAD + MEMORY_MIN_ALLOC ){
        return 0;
    }

    memman->begin_addr = (MemHeader*)begin_addr;
    memman->mem_total = memsize;
    init_memhdr( memman->begin_addr, memman->mem_total );
}

void* mem_alloc( uint32_t size )
{
    MemManager* memman = get_memman();
    MemHeader* hdr = memman->begin_addr;
    if( size < MEMORY_MIN_ALLOC ){
        size = MEMORY_MIN_ALLOC;
    }

    do { 
        if( !hdr->used && hdr->size >= size ){
            if( hdr->size >= MEMORY_MIN_ALLOC*2 + MEMORY_HDR_OVERHEAD ){
                hdr = mem_split( hdr, size );
            }
            hdr->used = 1;
            break;
        }
    } while( hdr = mem_nexthdr( hdr ) );

    return (uint8_t*)hdr + sizeof(MemHeader);
}

void mem_free( void* ptr )
{
    uint8_t* p = ptr;
    MemHeader* hdr = (MemHeader*)(p - sizeof(MemHeader));
    if( !ptr || !hdr->used ){
        return;
    }

    hdr->used = 0;
    MemHeader* prev = mem_prevhdr( hdr );
    MemHeader* next = mem_nexthdr( hdr );
    if( prev && !prev->used ){
        hdr = mem_merge( prev, hdr );
    }
    if( next && !next->used ){
        hdr = mem_merge( hdr, next );
    }
}

uint32_t mem_free_total(void)
{
    MemHeader* hdr = get_memman()->begin_addr;
    uint32_t free_size = 0;
    do {
        if( !hdr->used ){
            free_size += hdr->size;
        }
    } while( hdr = mem_nexthdr(hdr) );

    return free_size;
}

uint32_t _memtest_sub( uint32_t start, uint32_t end );
uint32_t memtest( uint32_t start, uint32_t end )
{
    char flg486 = 0;
    uint32_t eflg, cr0, endaddr;

    // 386か、486以降なのかの確認
    eflg = _io_load_eflags();
    eflg |= EFLAGS_AC_BIT;
    _io_store_eflags( eflg );
    eflg = _io_load_eflags();
    if( eflg & EFLAGS_AC_BIT ){    // 386ではAC=1にしても自動で0に戻ってしまう
        flg486 = 1;
    }
    eflg &= ~EFLAGS_AC_BIT;
    _io_store_eflags( eflg );

    if( flg486 ){
        cr0 = _io_load_cr0();
        cr0 |= CR0_CACHE_DISABLE;
        _io_store_cr0( cr0 );
    }

    endaddr = _memtest_sub( start, end );

    if( flg486 ){
        cr0 = _io_load_cr0();
        cr0 &= ~CR0_CACHE_DISABLE;
        _io_store_cr0( cr0 );
    }

    return endaddr;
}
