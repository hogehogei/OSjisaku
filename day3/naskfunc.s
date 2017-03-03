
.arch i486

.global _io_hlt
.global _write_mem8

.text 

_io_hlt:	# void _io_hlt(void);
	hlt
	ret

_write_mem8:	# void _write_mem8( int addr, int data );
	movl	4(%esp), %ecx
	movb	8(%esp), %al
	movb	%al, (%ecx)
	ret

