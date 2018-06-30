CC = gcc
CFLAGS = -ffreestanding -nostdlib -static -fno-stack-protector -m32 -fno-PIC -g
LDFLAGS = -Wl,-melf_i386
FILES = $(wildcard ../pdclib-master/*)

GRUB = $(HOME)/prog/grub/bin/
MKRESCUE = env PATH=$$PATH:$(GRUB) grub-mkrescue

boot.img: a.out user.out
	mkdir -p _boot/boot/grub
	cp a.out _boot
	cp textSection.out _boot
	cp dataSection.out _boot
	cp grub.cfg _boot/boot/grub/
	$(MKRESCUE) -o $@ _boot
	rm -rf _boot

a.out: boot.S io.S paging.S Common.c serialPort.c pageAllocator.c memoryAllocator.c gdt.c idt.c setGdt.S kernel_syscall.S kernel_syscall.c switchToUser.S kernel.c libpdc.a umoddivdi3.a
	$(CC) -o $@ -T linkscript $(CFLAGS) $(LDFLAGS) $^

user.out: user/helloWorld.c user/libpdc-user.a user/syscall.S umoddivdi3.a memoryAllocator.c pageAllocator.c Common.c
	$(CC) -o $@ -T user/linkscript $(CFLAGS) $(LDFLAGS) $^
	objcopy --dump-section .text=textSection.out user.out
	objcopy --dump-section .data=dataSection.out user.out


user/libpdc-user.a: ../pdclib-user $(FILES)
	rm -f libpdc-user.a
	rm -rf ../pdclib-user/build
	cd ../pdclib-user && mkdir build && cd build && cmake .. && make && cp libpdc.a ../../multiboot/user/libpdc-user.a

libpdc.a: ../pdclib-master $(FILES)
	rm -f user/libpdc-user.a
	rm -rf ../pdclib-master/build
	cd ../pdclib-master && mkdir build && cd build &&	cmake .. &&	make && cp libpdc.a ../../multiboot/


test: boot.img
	qemu-system-i386 -serial stdio -cdrom boot.img

clean:
	rm -f boot.img a.out
	rm -f libpdc.a
	rm -f user/libpdc-user.a
	rm -f user.out
	rm -f textSection.out
	rm -f dataSection.out
	rm -rf ../pdclib-user/build
	rm -rf ../pdclib-master/build
