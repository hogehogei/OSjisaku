
.code16

.equ    CYLS, 10
.text
	jmp entry
	.byte 0x90
	.ascii "HOGEIIPL" # ブートセクタの名前を自由に書いてよい
	.word 512         # 1セクタの大きさ（512にしなければいけない）
	.byte 1           # クラスタの大きさ（1セクタにしなければいけない）
	.word 1           # FATがどこから始まるか（普通は1セクタ目からにする）
	.byte 2           # FATの個数（2にしなければいけない）
	.word 224         # ルートディレクトリ領域の大きさ（普通は224エントリにする）
	.word 2880        # このドライブの大きさ（2880セクタにしなければいけない）
	.byte 0xf0        # メディアのタイプ（0xF0にしなければいけない）
	.word 9           # FAT領域の長さ（9セクタにしなければいけない）
	.word 18          # 1トラックにいくつのセクタがあるか（18にしなければいけない）
	.word 2           # ヘッドの数（2にしなければいけない）
	.int 0            # パーティションを使っていないので必ず0
	.int 2880         # このドライブの大きさをもう一度書く
	.byte 0, 0, 0x29  # よくわからないけどこの値にしておくといいらしい
	.int 0xffffffff   # たぶんボリュームシリアル番号
	.ascii "HELLO-OS   "    # ディスクの名前　11バイト
	.ascii "FAT12   "       # フォーマットの名前　8バイト
	.skip 18          # 18バイトあけておく

entry:
	movw $0, %ax
	movw %ax, %ss
	movw $0x7c00, %sp
	movw %ax, %ds


# ディスクを読む
	movw $0x0820, %ax
	movw %ax, %es
	movb $0, %ch      # シリンダ0
	movb $0, %dh      # ヘッド0
	movb $2, %cl      # セクタ2

readloop:
	movw $0, %si     # 失敗回数を数えるレジスタ

retry:
	movb $0x02, %ah   # AH=0x02 ディスク読み込み
	movb $1, %al      # 1セクタ
	movw $0, %bx
	movb $0x00, %dl   # Aドライブ
	int  $0x13        # ディスクBIOS呼び出し
	jnc  next
	addw $1, %si
	cmp  $5, %si
	jae  error
	movb $0x00, %ah
	movb $0x00, %dl   # Aドライブ
	int  $0x13        # ドライブのリセット
	jmp  retry
next:
	movw %es, %ax     # アドレスを0x0200 進める
	                  # addw 0x0020, %es という命令がないのでこうしている
	addw $0x0020, %ax
	movw %ax, %es
	addb $1, %cl
	cmp  $18, %cl     # 18セクタまで読むので18以下だったらループ
	jbe  readloop
	movb $1, %cl
	addb $1, %dh      # 次のヘッダ
	cmp  $2, %dh
	jb   readloop
	movb $0, %dh  
	addb $1, %ch      # 次のシリンダ
	cmp  $CYLS, %ch
	jb   readloop
	
	movb %ch, (0x0ff0)       # IPLがどこまで読んだのかメモ
	jmp  0xc200              # すべて読み終わった asmhead.s へ

# 読み終わったけどやることないので寝る
fin:
	hlt
	jmp fin

success:
	movw $msgok, %si
	jmp  putloop
error:
	movw $msgerr, %si
	jmp  putloop
putloop:
	movb (%si), %al # 書き込む文字コード
	addw $1, %si    # 次のアドレスに進める
	cmpb $0, %al    # al == 0 なら終了
	je   fin
	movb $0x0e, %ah # 1文字表示ファンクション
	movw $15, %bx   # カラーコード
	int  $0x10      # ビデオBIOS呼び出し
	jmp  putloop
msgerr:
	.byte 0x0a, 0x0a
	.ascii "reading error"
	.byte 0x0a, 0x00
msgok:
	.byte 0x0a, 0x0a
	.ascii "reading sectors succeeded!"
	.byte 0x0a, 0x00

