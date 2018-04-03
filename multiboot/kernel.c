#include "multiboot2.h"
#include "pageAllocator.h"
// #include "memoryAllocator.h"
#include <stdlib.h>
#include "Common.h"
#include "serialPort.h"
#include <stdio.h>

void main(unsigned long magic, unsigned long addr) {
  clear();

  if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
    vga_puts("invalid magic number :-(");
    return;
  }

  if (addr & 7) {
    vga_puts("unaligned mbi :-(");
    return;
  }

  vga_puts("hello world, i guess?");

  addr = addr + 8;
  struct multiboot_tag *tag = addr;
  while (tag->type != 0) {
    if (tag->type == MULTIBOOT_TAG_TYPE_MODULE) {
      vga_puts("content of module:");
      struct multiboot_tag_module *module = (struct multiboot_tag_module *)tag;
      char *byte = (char *)module->mod_start;
      while (byte != module->mod_end) {
        vga_putchar(*byte);
        ++byte;
      }
    } else if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
      //TODO set some memory frames as reserved...
      struct multiboot_tag_mmap *mmap = (struct multiboot_tag_mmap *)tag;
      multiboot_memory_map_t *entry = mmap->entries;
      while (entry < tag + tag->size) {
        if (entry->type == MULTIBOOT_MEMORY_RESERVED && entry->addr > 4 * 1024 * 1024) { // SKIP  FIRST 20 MEGABYTE, which is identity mapped
          for (int i = entry->addr; (i < (entry->addr + entry->len)) && (i < PAGES * 1024); i++) {
            unsigned int index = (i & 0xfffff000) / 0x1000;
            bitmap[index/32] |= (1 << (index % 32));
          }
        }
        entry++;
      }
    }
    addr = (addr + (tag->size) + 7) & (~7);
    tag = addr;
  }

  for (int i = 0; i < PAGES/32; i++) {
    if (bitmap[i] != 0) {
      vga_puts("reserved frames");
    }
  }

  for (unsigned int i = 0; i < 64; ++i) {
    page_directory[i] = ((unsigned long) page_tables[i] | 3);
  }

  for (int i = 64; i < 1024; ++i) {
    page_directory[i] = 0x00000002;
  }

  for (unsigned int i = 0; i < 5120; ++i) {
    allocatePage();
  }

  loadPageDirectory(page_directory);
  enablePaging();

  printf("%s printf funguje :) ... Aj si cislami: %d\n", "Ahoj,", 22);

}
