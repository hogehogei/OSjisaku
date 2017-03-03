
void _io_hlt(void);
void _io_cli(void);
void _write_mem8( int addr, int data );
int _io_load_eflags(void);
void _io_store_eflags( int eflags );

void init_pallet(void);
void set_pallet( int start, int end, unsigned char* rgb );

void HogeMain(void)
{
    int i = 0;
    char* p = 0;

    init_pallet();

    for( i = 0; i <= 0xFFFF; ++i ){
        p[i] = i & 0x0F;
    }

    while(1){
        _io_hlt();
    }
}

void init_pallet(void)
{
    static unsigned char skTableRGB[16*3] = {
        0x00, 0x00, 0x00,    // black
        0xFF, 0x00, 0x00,    // red
        0x00, 0xFF, 0x00,    // green
        0xFF, 0xFF, 0x00,    // yellow
        0x00, 0x00, 0xFF,    // blue
        0xFF, 0x00, 0xFF,    // purple
        0x00, 0xFF, 0xFF,    // light blue
        0xFF, 0xFF, 0xFF,    // white
        0xc6, 0xc6, 0xc6,    // light gray
        0x84, 0x00, 0x00,    // dark red
        0x00, 0x84, 0x00,    // dark green
        0x00, 0x00, 0x84,    // dark blue
        0x84, 0x00, 0x84,    // dark purple
        0x00, 0x84, 0x84,    // deep blue
        0x84, 0x84, 0x84,    // dark gray
    };

    set_pallet( 0, 15, skTableRGB );
}

void set_pallet( int start, int end, unsigned char* rgb )
{
    int i, eflags;
    eflags = _io_load_eflags();    // 割り込み許可フラグの値を記録
    _io_cli();    // 割り込み禁止

    _io_out( 0xc308, start );
    for( i = start; i <= end; ++i ){
        _io_out( 0xc309, rgb[0] / 4 );
        _io_out( 0xc309, rgb[1] / 4 );
        _io_out( 0xc309, rgb[2] / 4 );
        rgb += 3;
    }
    _io_store_eflags( eflags );    // 割り込み許可
}
