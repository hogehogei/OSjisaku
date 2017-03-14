
.code16
.text

.equ	BOTPAK, 0x00280000	# bootpackのロード先
.equ	DSKCAC, 0x00100000	# ディスクキャッシュの場所
.equ	DSKCAC0, 0x00008000	# ディスクキャッシュの場所（リアルモード）

# BOOT_INFO 関係
.equ	CYLS, 0x0ff0	# ブートセクタが設定する
.equ	LEDS, 0x0ff1
.equ	VMODE, 0x0ff2	# 色数に関する情報。何ビットカラーか？
.equ	SCRNX, 0x0ff4	# 解像度のX
.equ	SCRNY, 0x0ff6	# 解像度のY
.equ	VRAM, 0x0ff8	# グラフィックバッファの開始番地

	# VGAグラフィックス、320x200x8bit カラーに設定
	movb $0x13, %al
	movb $0x00, %ah
	int  $0x10

	movb $8, (VMODE)    # 画面モードをメモする（C言語が参照する）
	movw $320, (SCRNX)  
	movw $200, (SCRNY)
	movl $0x000a0000, (VRAM)

	# キーボードのLED状態をBIOSに教えてもらう
	movb $0x02, %ah
	int  $0x16
	movb %al, (LEDS)

	# PICが一切の割り込みを受け付けないようにする
	# AT互換機の仕様では、PICの初期化をするなら
	# こいつをCLI前にやっておかないと、たまにハングする
	# PICの初期化はあとでやる
	movb $0xff, %al
	out  %al, $0x21
	nop			# OUT命令を連続させるとうまくいかない機種があるらしいので	
	out  %al, $0xa1
	cli			# さらにCPUレベルでも割り込み禁止

	# CPUから1MB以上のメモリにアクセスできるように、A20GATEを設定
	call waitkbdout
	movb $0xd1, %al
	outb %al, $0x64
	call waitkbdout
	movb $0xdf, %al
	outb %al, $0x60
	call waitkbdout
	
.arch i486
	lgdtl (GDTR0)	# 暫定GDTを設定
	movl  %cr0, %eax
	andl  $0x7fffffff, %eax	# bit31を0にする（ページング禁止のため）
	orl   $0x00000001, %eax	# bit0を1にする（プロテクトモード移行のため）
	movl  %eax, %cr0 
	jmp   pipelineflush
pipelineflush:
	movw  $1*8, %ax	#読み書き可能セグメント32bit
	movw  %ax, %ds
	movw  %ax, %es
	movw  %ax, %fs
	movw  %ax, %gs
	movw  %ax, %ss

	movl  $bootpack, %esi
	#movl  $0xc330, %esi
	movl  $BOTPAK, %edi
	movl  $512*1024/4, %ecx
	call  memcpy

	# ついでにディスクデータも本来の位置に転送
	# まずはブートセクタから
	movl  $0x7c00, %esi
	movl  $DSKCAC, %edi
	movl  $512/4, %ecx
	call  memcpy

	# 残り全部
	movl  $DSKCAC0+512, %esi
	movl  $DSKCAC+512, %edi
	movl  $0x00, %ecx
	movb  (CYLS), %cl
	imull $512*18*2/4, %ecx	# シリンダ数からバイト数/4に変更
	subl  $512/4, %ecx	# IPLの分だけ差し引く
	call  memcpy

	# asmheadでしなければいけないことは全部し終わったので、
	# あとは bootpack に任せる
	movl  $BOTPAK, %ebx
	# .data を スタックに転送しておく
	movl  16(%ebx), %ecx
	addl  $3, %ecx
	shrl  $2, %ecx	# %ecx /= 4  
	jz    skip	# .data の転送するべきものがない
	movl  20(%ebx), %esi
	addl  %ebx, %esi
	movl  12(%ebx), %edi
	call  memcpy
skip:
	movl  12(%ebx), %esp    # スタック初期値
	ljmpl $2*8, $0x0000001b
	

waitkbdout:
	inb   $0x64, %al
	andb  $0x02, %al
	jnz   waitkbdout
	ret

# memcpy
# %esi   転送元
# %edi   転送先
# %ecx   転送長 4byteずつ転送するので 転送長/4して渡す
memcpy:
	movl (%esi), %eax
	addl $4, %esi
	movl %eax, (%edi)
	addl $4, %edi
	subl $1, %ecx
	jnz  memcpy
	ret
	
.align  16
GDT0:
.skip	8, 0x00				# ヌルセレクタ
.word	0xffff, 0x0000, 0x9200, 0x00cf	# 読み書き可能セグメント32bit
.word	0xffff, 0x0000, 0x9a28, 0x0047	# 実行可能セグメント32bit（bootpack用）
.word	0x0000

GDTR0:
.word	8*3-1
.int	GDT0

.align  16
bootpack:
