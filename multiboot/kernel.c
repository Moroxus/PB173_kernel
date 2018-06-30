#include "multiboot2.h"
#include "pageAllocator.h"
#include <stdlib.h>
#include "Common.h"
#include "serialPort.h"
#include <stdio.h>
#include "gdt.h"
#include "idt.h"
#include "kernel_syscall.h"
#include <string.h>
#include "switchToUser.h"


void main(unsigned long magic, unsigned long addr) {
  clear();
  char source[5] = {'t','e','x','s','\0'};
  char dest[5] = {0};

  if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
    vga_puts("invalid magic number :-(");
    return;
  }

  if (addr & 7) {
    vga_puts("unaligned mbi :-(");
    return;
  }

  vga_puts("hello world, i guess?");

  for (unsigned int i = 0; i < 64; ++i) {
    page_directory[i] = ((unsigned long) page_tables[i] | 3);
  }

  for (int i = 64; i < 1024; ++i) {
    page_directory[i] = 0x00000002;
  }
  page_directory[5] |= 7; // these pages are for userland

  for (unsigned int i = 0; i < 6144; ++i) {
    allocatePage();       // identity maping for first 6144 pages, userland uses pagues above 5120
  }

  for (int i = 0; i < 1024; ++i) {
    page_tables[5][i] |= 7;   // pages for userland
  }


  loadPageDirectory(page_directory);
  enablePaging();

  unsigned user_stack = 0x017FFFFF; // stack for userland

  addr = addr + 8;
  struct multiboot_tag *tag = addr;
  while (tag->type != 0) {
    if (tag->type == MULTIBOOT_TAG_TYPE_MODULE) {
      vga_puts("content of module:");
      struct multiboot_tag_module *module = (struct multiboot_tag_module *)tag;
      // printf("%s\n", module->cmdline);
      vga_puts(module->cmdline);
      if (strcmp(module->cmdline, "exec.text") == 0) {
        printf("Size of text is %u\n", module->mod_end - module->mod_start);
        memcpy((void *)0x01400000, module->mod_start, module->mod_end - module->mod_start);
      }
      if (strcmp(module->cmdline, "exec.data") == 0) {
        memcpy(0x0140A000, module->mod_start, module->mod_end - module->mod_start);
        printf("Size of data is %u\n", module->mod_end - module->mod_start);
      }
    } else if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
      //TODO set some memory frames as reserved...
      struct multiboot_tag_mmap *mmap = (struct multiboot_tag_mmap *)tag;
      multiboot_memory_map_t *entry = mmap->entries;
      while (entry < tag + tag->size) { // should skip reserver memory, but actually, in these parts is nothing reserved...
        if (entry->type == MULTIBOOT_MEMORY_RESERVED && entry->addr > 4 * 1024 * 1024) { // SKIP  FIRST 4 MEGABYTE, which is identity mapped
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


  GDT[0] = create_descriptor(0, 0, 0);
  GDT[1] = create_descriptor(0, 0x000FFFFF, 0xC09A); // CODE
  GDT[2] = create_descriptor(0, 0x000FFFFF, 0xC092); // DATA
  GDT[3] = create_descriptor(0, 0x000FFFFF, 0xC0FA); // CODE
  GDT[4] = create_descriptor(0, 0x000FFFFF, 0xC0F2); // DATA
  struct dtr gdt_tuple = {GDT_SIZE*8 - 1, (uint32_t)&(GDT[0])};
  struct dtr idt_tuple = {IDT_SIZE*8 - 1, (uint32_t)&(IDT[0])};


// this basically will not be used, because TSS will be setup to kernel stack in switchToUser() function
  unsigned int interrupt_stack = allocatePage();
  interrupt_stack += 4 * 1024 -1;
  printf("interrupt_stack is %u\n", interrupt_stack); // interrupt stack will not be used
  uint32_t TSS[26] = {0};
  TSS[1] = interrupt_stack;
  TSS[2] = 0x10;
  TSS[25] = (104) << 15;
  GDT[5] = create_descriptor(&TSS, sizeof(TSS), 0x89);

  IDT[128] = create_idt_entry((uint32_t)&Int128Handler);
  IDT[129] = create_idt_entry((uint32_t)&Int129Handler);
  setGdt((uint32_t)&gdt_tuple);
  setIdt((uint32_t)&idt_tuple);
  setTSS();


  switchToUser(0x01400000, user_stack, &TSS[1]); // last parameter is pointer to TSS[1] where is stack pointer; this is so kernel stack pointer can be placed to TSS[1]

  printf("Back in kernel my friend :D\n");

}
