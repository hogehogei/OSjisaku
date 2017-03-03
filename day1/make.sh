as ipl.s -o ipl.o
objcopy -R .note.gnu.build-id -S -O binary ipl.o helloos.img
