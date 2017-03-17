
.arch i486
.code32

.global _io_hlt
.global _io_cli
.global _io_sti, _io_stihlt
.global _io_in8, _io_in16, _io_in32
.global _io_out8, _io_out16, _io_out32
.global _io_load_eflags, _io_store_eflags
.global _io_load_cr0, _io_store_cr0
.global	_load_gdtr, _load_idtr
.global _asm_inthandler21, _asm_inthandler2c
.global _memtest_sub

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

_io_load_cr0:		# int _io_load_cr0(void);
	movl	%cr0, %eax
	ret

_io_store_cr0:		# void _io_store_cr0( int cr0 );
	movl	4(%esp), %eax
	movl	%eax, %cr0
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

_memtest_sub:	# uint32_t memtest_sub( uint32_t start, uint32_t end );
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	movl	$0xAA55AA55, %esi    # pat0 = 0xAA55AA55
	movl	$0x55AA55AA, %edi    # pat1 = 0x55AA55AA
	movl	16(%esp), %eax  # start をスタックから取り出す
mts_loop:
	movl	%eax, %ebx      
	addl	$0xFFC, %ebx           # i += 0xFFC;
	movl	(%ebx), %edx           # old = *p;
	movl	%esi, (%ebx)           # *p = pat0
	xorl	$0xFFFFFFFF, (%ebx)    # *p ^= 0xFFFFFFFF
	cmpl	%edi, (%ebx)	       # if( *p != pat1 ) goto mts_end;
	jne	mts_end
	xorl	$0xFFFFFFFF, (%ebx)    # *p ^= 0xFFFFFFFF
	cmpl	%esi, (%ebx)           # if( *p != pat0 ) goto mts_end;
	jne	mts_end
	movl	%edx, (%ebx)	       # *p = old;
	addl	$0x1000, %eax          # i += 0x1000;
	cmpl	20(%esp), %eax         # if( i <= end )
	jbe	mts_loop
mts_end:
	movl	%edx, (%ebx)           # *p = old;
	popl	%ebx
	popl	%esi
	popl	%edi
	ret

# Keyboard 入力割り込み処理
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
	
# mouse 入力割り込み処理
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
