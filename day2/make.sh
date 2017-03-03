as ipl.s -o ipl.o
ld -o ipl.bin -T lnk.ld ipl.o
mformat -f 1440 -C -B ipl.bin -i helloos.img ::
#objcopy -R .note.gnu.build-id -S -O binary ipl.o helloos.img
