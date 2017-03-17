#ifndef    BOOTPACK_H_INCLUDED
#define    BOOTPACK_H_INCLUDED

void _io_hlt(void);
void _io_cli(void);

void _io_out8( int port, int data );

int _io_load_eflags(void);
void _io_store_eflags( int eflags );
int _io_load_cr0(void);
void _io_store_cr0( int cr0 );

#endif
