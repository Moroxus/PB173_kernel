#include "multiboot2.h"
#include "pageAllocator.h"
#include "memoryAllocator.h"
#include "Common.h"
#include "serialPort.h"

void main(unsigned long magic, unsigned long addr) {
  clear();

  if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
    puts("invalid magic number :-(");
    return;
  }

  if (addr & 7) {
    puts("unaligned mbi :-(");
    return;
  }

  puts("hello world, i guess?");

  addr = addr + 8;
  struct multiboot_tag *tag = addr;
  while (tag->type != 0) {
    if (tag->type == MULTIBOOT_TAG_TYPE_MODULE) {
      puts("content of module:");
      struct multiboot_tag_module *module = tag;
      char *byte = module->mod_start;
      while (byte != module->mod_end) {
        putchar(*byte);
        ++byte;
      }
    }
    addr = (addr + (tag->size) + 7) & (~7);
    tag = addr;
  }

  for (unsigned int i = 0; i < 64; ++i) {
    page_directory[i] = ((unsigned long) page_tables[i] | 3);
  }

  for (int i = 64; i < 1024; ++i) {
    page_directory[i] = 0x00000002;
  }

  for (unsigned int i = 0; i < 1024; ++i) {
    allocatePage();
  }

  loadPageDirectory(page_directory);
  enablePaging();

  puts("Write something to conemu console, it is then read by inb() function "
       "and after that wrote by outb function to conemu console");

  init_serial();
  while (1) {
    write_serial(read_serial());
  }
}
