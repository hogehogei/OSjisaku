
#ifndef INTERRUPT_H_INCLUDED
#define INTERRUPT_H_INCLUDED

void init_pic(void);

#define PIC0_ICW1    0x0020
#define PIC0_OCW2    0x0020
#define PIC0_IMR     0x0021
#define PIC0_ICW2    0x0021
#define PIC0_ICW3    0x0021
#define PIC0_ICW4    0x0021
#define PIC1_ICW1    0x00A0
#define PIC1_OCW2    0x00A0
#define PIC1_IMR     0x00A1
#define PIC1_ICW2    0x00A1
#define PIC1_ICW3    0x00A1
#define PIC1_ICW4    0x00A1

// _asm_inthandlerXX はアセンブラで記述
// レジスタ退避後に本物の _inthandlerXX_　を呼び出す
// 割り込みハンドラには _asm_inthandlerXX を登録すること
void _asm_inthandler21(void);
void _inthandler21_keyboard( int* esp );
void _asm_inthandler2c(void);
void _inthandler2c_mouse( int* esp );

#endif

