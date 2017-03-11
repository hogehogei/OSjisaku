
.arch i486
.code32

.global _io_hlt
.global _io_cli
.global _io_sti, _io_stihlt
.global _io_in8, _io_in16, _io_in32
.global _io_out8, _io_out16, _io_out32
.global _io_load_eflags, _io_store_eflags
.global	_load_gdtr, _load_idtr
.global _asm_inthandler21, _asm_inthandler2c

.extern	_inthandler21_keyboard, _inthandler2c_mouse

.text 

_io_hlt:	# void _io_hlt(void);
	hlt
	ret

_io_cli:	# void _io_cli(void);
	cli
	ret

_io_sti:	# void _io_sti(void);
	sti
	ret

_io_stihlt:	# void _io_stihlt(void);
	sti
	hlt
	ret

_io_in8:	# int _io_in8(int port);
	movl	4(%esp), %edx
	movl	$0, %eax
	inb	%dx, %al
	ret

_io_in16:	# int _io_in16(int port);
	movl	4(%esp), %edx
	movl 	$0, %eax
	inw	%dx, %ax
	ret

_io_in32:	# int _io_in32(int port);
	movl	4(%esp), %edx
	inl	%dx, %eax
	ret

_io_out8:	# void _io_out8( int port, int data );
	movl	4(%esp), %edx	# port
	movb	8(%esp), %al	# data
	outb	%al, %dx
	ret

_io_out16:	# void _io_out16( int port, int data );
	movl	4(%esp), %edx
	movl	8(%esp), %eax
	outw	%ax, %dx
	ret

_io_out32:	# void _io_out32( int port, int data );
	movl	4(%esp), %edx
	movl	8(%esp), %eax
	outl	%eax, %dx
	ret

_io_load_eflags:	# int _io_load_eflags(void);
	pushfl
	pop 	%eax
	ret

_io_store_eflags:	# void _io_store_eflags( int eflags );
	movl	4(%esp), %eax
	push	%eax
	popfl
	ret

_load_gdtr:	# void load_gdtr( int limit, int addr );
	movw	4(%esp), %ax
	movw	%ax, 6(%esp)
	lgdt	6(%esp)
	ret

_load_idtr:	# void load_idtr( int limit, int addr );
	movw	4(%esp), %ax
	movw	%ax, 6(%esp)
	lidt	6(%esp)
	ret

// Keyboard 入力割り込み処理
_asm_inthandler21:
	pushw	%es
	pushw	%ds
	pusha	
	movl	%esp, %eax
	pushl	%eax
	movw	%ss, %ax
	movw	%ax, %ds
	movw	%ax, %es
	call	_inthandler21_keyboard
	popl	%eax
	popa
	popw	%ds
	popw	%es
	iret
	
// mouse 入力割り込み処理
_asm_inthandler2c:
	pushw	%es
	pushw	%ds
	pusha	
	movl	%esp, %eax
	pushl	%eax
	movw	%ss, %ax
	movw	%ax, %ds
	movw	%ax, %es
	call	_inthandler2c_mouse
	popl	%eax
	popa
	popw	%ds
	popw	%es
	iret
