OUTPUT_FORMAT("binary");

SECTIONS {
. = 0x7c00;
.ipl_text : { ipl10.o(.text) }

. = 0x7dfe;
.sig : { SHORT(0xaa55); }
}
