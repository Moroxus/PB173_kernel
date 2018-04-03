CC = gcc
CFLAGS = -ffreestanding -nostdlib -static -fno-stack-protector -m32 -fno-PIC
LDFLAGS = -Wl,-melf_i386
FILES = $(wildcard ../pdclib-master/*)

GRUB = $(HOME)/prog/grub/bin/
MKRESCUE = env PATH=$$PATH:$(GRUB) grub-mkrescue

boot.img: a.out
	mkdir -p _boot/boot/grub
	cp a.out _boot
	cp ahoj.txt _boot
	cp grub.cfg _boot/boot/grub/
	$(MKRESCUE) -o $@ _boot
	rm -rf _boot

a.out: boot.S io.S paging.S Common.c serialPort.c pageAllocator.c memoryAllocator.c kernel.c libpdc.a umoddivdi3.a
	$(CC) -o $@ -T linkscript $(CFLAGS) $(LDFLAGS) $^

libpdc.a: ../pdclib-master $(FILES)
	rm -f libpdc.a
	rm -rf ../pdclib-master/build
	cd ../pdclib-master && mkdir build && cd build &&	cmake .. &&	make && cp libpdc.a ../../multiboot/


test: boot.img
	qemu-system-i386 -serial stdio -cdrom boot.img

clean:
	rm -f boot.img a.out
	rm -f libpdc.a
	rm -rf ../pdclib-master/build
