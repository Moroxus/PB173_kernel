CC = gcc
CFLAGS = -ffreestanding -nostdlib -static -fno-stack-protector -m32 -fno-PIC
LDFLAGS = -Wl,-melf_i386
GRUB = $(HOME)/prog/grub/bin/
MKRESCUE = env PATH=$$PATH:$(GRUB) grub-mkrescue

boot.img: a.out
	mkdir -p _boot/boot/grub
	cp a.out _boot
	cp ahoj.txt _boot
	cp grub.cfg _boot/boot/grub/
	$(MKRESCUE) -o $@ _boot
	rm -rf _boot

a.out: boot.S kernel.c
	$(CC) -o $@ -T linkscript $(CFLAGS) $(LDFLAGS) boot.S kernel.c  

test: boot.img
	qemu-system-i386 -cdrom boot.img

clean:
	rm -f boot.img a.out
