
all: ipl.o lnk.ld 
	ld -o ipl.bin -T lnk.ld ipl.o
	mformat -f 1440 -C -B ipl.bin -i helloos.img ::
ipl.o:
	as ipl.s -o ipl.o
clean:
	rm ipl.bin helloos.img ipl.o
run: all
	cp helloos.img ../
	virsh start osjisaku
stop:
	virsh destroy osjisaku
	
