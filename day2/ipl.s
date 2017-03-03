
.code16
.org 0
.text
	jmp entry
	.byte 0x90
	.ascii "HELLOIPL"
	.word 512
	.byte 1
	.word 1
	.byte 2
	.word 224
	.word 2880
	.byte 0xf0
	.word 9
	.word 18
	.word 2
	.int 0
	.int 2880
	.byte 0
	.byte 0
	.byte 0x29
	.int 0xffffffff
	.ascii "HELLO-OS   "
	.ascii "FAT12   "
	.skip 18

entry:
	movw $0, %ax
	movw %ax, %ss
	movw $0x7c00, %sp
	movw %ax, %ds
	movw %ax, %es

	movw $msg, %si

putloop:
	movb (%si), %al # 書き込む文字コード
	addw $1, %si    # 次のアドレスに進める
	cmpb $0, %al    # al == 0 なら終了
	je fin
	movb $0x0e, %ah # 1文字表示ファンクション
	movw $15, %bx # カラーコード
	int $0x10       # ビデオBIOS呼び出し
	jmp putloop
fin:
	hlt
	jmp fin
msg:
	.byte 0x0a, 0x0a
	.ascii "HELLOOOOOOOO"
	.byte 0x0a, 0x00

#	.org 0x1fe
#	.byte 0x55, 0xaa

#	.byte 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
#	.skip 4600
#	.byte 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
#	.skip 1469432

