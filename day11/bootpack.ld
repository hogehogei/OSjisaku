OUTPUT_FORMAT("binary");
ENTRY("HogeMain");

SECTIONS {
.head 0x00  : {
	LONG(128*1024)         /* 0 : stack+.data+heap の大きさ(4KBの倍数）*/
	LONG(0x69726148)       /* 4 : シグネチャ "Hari" */
	LONG(0)                /* 8 : mmarea の大きさ（4KBの倍数）*/
	LONG(0x310000)         /* 12 : stack 初期値＆.data転送先 */
	LONG(SIZEOF(.data))    /* 16 : .data サイズ */
	LONG(LOADADDR(.data))  /* 20 : .data の初期値列のファイル位置 */
	LONG(0xE9000000)       /* 24 : 0xE9000000 */
	LONG(HogeMain - 0x20)  /* 28 : エントリアドレス - 0x20 */
	LONG(24 * 1024)        /* 32 : heap領域(malloc領域)　開始アドレス */
}

.text : 
{
	_text_start = .;
	bootpack.o(*.text)
	*(.text) 
	_text_end = .;
}

.data 0x00310000 : AT( ADDR(.text) + SIZEOF(.text) ) {
	*(.data)
	*(.rodata*)
	*(.bss)
}

.eh_frame : { *(.eh_frame) }

}
