
OSNAME=hogeios
IMG=$(OSNAME).img
APPOBJ=bootpack.o bootinfo.o graphic.o hankaku.o misc.o mouse.o dsctbl.o interrupt.o keyboard.o memory.o sheet.o
CCOPT=-O2 -m32 -nostdlib -Wl,--oformat=binary
ASOPT=--32

.SUFFIXES: .c .o

all: ipl10.bin asmhead.bin bootpack.bin
	cat asmhead.bin bootpack.bin > boot.bin
	mformat -f 1440 -C -B ipl10.bin -i hogeios.img ::
	mcopy boot.bin -i hogeios.img ::

ipl10.bin: ipl10.o ipl10.ld
	ld -o ipl10.bin -T ipl10.ld -m elf_i386 ipl10.o
asmhead.bin: asmhead.o asmhead.ld
	ld -o asmhead.bin -T asmhead.ld -m elf_i386 asmhead.o
bootpack.bin: bootpack.ld asmfunc.o $(APPOBJ)
	ld -o bootpack.bin -T bootpack.ld -m elf_i386 $(APPOBJ) asmfunc.o
	

ipl10.o: ipl10.s
	gcc $(CCOPT) -g -c -T ipl10.ld -Wa,-a,-ad ipl10.s -o ipl10.o > ipl10.lst
asmhead.o: asmhead.s
	gcc $(CCOPT) -g -c -T asmhead.ld -Wa,-a,-ad asmhead.s -o asmhead.o > asmhead.lst
asmfunc.o: asmfunc.s
	gcc $(CCOPT) -g -c -Wa,-a,-ad asmfunc.s -o asmfunc.o > asmfunc.lst
.c.o:
	gcc $(CCOPT) -g -c -T bootpack.ld -Wa,-a,-ad $< -o $@ > $<.lst
clean:
	rm *.bin *.img *.o *.lst
	rm ../hogeios.img

run: all
	cp hogeios.img ../
	virsh start osjisaku
stop:
	virsh destroy osjisaku
	
