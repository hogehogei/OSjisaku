OUTPUT_FORMAT("binary");

SECTIONS {
. = 0xc200;
.asmhead_text : { asmhead.o(.text) }
}
