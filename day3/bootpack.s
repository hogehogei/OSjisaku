	.file	"bootpack.c"
	.text
.Ltext0:
	.globl	HogeMain
	.type	HogeMain, @function
HogeMain:
.LFB0:
	.file 1 "bootpack.c"
	.loc 1 6 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	.loc 1 7 0
	movl	$0, -12(%rbp)
	.loc 1 8 0
	movq	$655360, -8(%rbp)
	.loc 1 9 0
	movl	$655360, -12(%rbp)
	jmp	.L2
.L3:
	.loc 1 10 0 discriminator 3
	movl	-12(%rbp), %eax
	movslq	%eax, %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	movb	$15, (%rax)
	.loc 1 9 0 discriminator 3
	addl	$1, -12(%rbp)
.L2:
	.loc 1 9 0 is_stmt 0 discriminator 1
	cmpl	$720895, -12(%rbp)
	jle	.L3
.L4:
	.loc 1 15 0 is_stmt 1 discriminator 1
	call	_io_hlt
	.loc 1 16 0 discriminator 1
	jmp	.L4
	.cfi_endproc
.LFE0:
	.size	HogeMain, .-HogeMain
.Letext0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x74
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF1
	.byte	0x1
	.long	.LASF2
	.long	.LASF3
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.long	.LASF4
	.byte	0x1
	.byte	0x5
	.quad	.LFB0
	.quad	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.long	0x63
	.uleb128 0x3
	.string	"i"
	.byte	0x1
	.byte	0x7
	.long	0x63
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x3
	.string	"p"
	.byte	0x1
	.byte	0x8
	.long	0x6a
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x4
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x5
	.byte	0x8
	.long	0x70
	.uleb128 0x6
	.byte	0x1
	.byte	0x6
	.long	.LASF0
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0
	.value	0
	.value	0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0
	.quad	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF2:
	.string	"bootpack.c"
.LASF1:
	.string	"GNU C 4.9.4 -mtune=generic -march=x86-64 -g -fstack-protector-strong"
.LASF4:
	.string	"HogeMain"
.LASF3:
	.string	"/home/ryo/OSJisaku/day3"
.LASF0:
	.string	"char"
	.ident	"GCC: (Gentoo 4.9.4 p1.0, pie-0.6.4) 4.9.4"
	.section	.note.GNU-stack,"",@progbits
