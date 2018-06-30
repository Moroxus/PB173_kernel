CC = gcc
PDC = ../pdclib-master
PDC_PLATFORM = $(PDC)/platform/example
INC =  -I$(PDC)/includes -I$(PDC)/internals -I$(PDC_PLATFORM)/includes -I$(PDC_PLATFORM)/internals
CFLAGS = -ffreestanding -nostdlib -static -fno-stack-protector -m32 -fno-PIC -g -std=gnu99 $(INC) -no-pie
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
	$(CC) -o $@ -Wl,-T,linkscript $(CFLAGS) $(LDFLAGS) $^

user.out: user/helloWorld.c user/libpdc-user.a user/syscall.S umoddivdi3.a memoryAllocator.c pageAllocator.c Common.c
	$(CC) -o $@ -Wl,-T,user/linkscript $(CFLAGS) $(LDFLAGS) $^
	objcopy -O binary -j .text -S user.out textSection.out
	objcopy -O binary -j .data -S user.out dataSection.out


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
