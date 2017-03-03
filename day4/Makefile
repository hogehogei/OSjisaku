
OSNAME=hogeios
IMG=$(OSNAME).img
CCOPT=-O0 -m32 -nostdlib -Wl,--oformat=binary
ASOPT=--32

all: ipl10.o asmhead.o bootpack.o naskfunc.o
	ld -o ipl10.bin -T ipl10.ld -m elf_i386 ipl10.o
	ld -o asmhead.bin -T asmhead.ld -m elf_i386 asmhead.o
	ld -o bootpack.bin -T bootpack.ld -m elf_i386 bootpack.o naskfunc.o
	cat asmhead.bin bootpack.bin > boot.bin
	mformat -f 1440 -C -B ipl10.bin -i hogeios.img ::
	mcopy boot.bin -i hogeios.img ::

ipl10.o: 
	gcc $(CCOPT) -g -c -T ipl10.ld -Wa,-a,-ad ipl10.s -o ipl10.o > ipl10.lst
asmhead.o: 
	gcc $(CCOPT) -g -c -T asmhead.ld -Wa,-a,-ad asmhead.s -o asmhead.o > asmhead.lst
naskfunc.o:
	as $(ASOPT) naskfunc.s -o naskfunc.o
	as $(ASOPT) naskfunc.s -a -ad > naskfunc.lst
bootpack.o:
	gcc $(CCOPT) -g -c -T bootpack.ld -Wa,-a,-ad bootpack.c -o bootpack.o > bootpack.lst
clean:
	rm *.bin *.img *.o *.lst
	rm ../hogeios.img

run: all
	cp hogeios.img ../
	virsh start osjisaku
stop:
	virsh destroy osjisaku
	
