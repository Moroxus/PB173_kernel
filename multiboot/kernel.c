#include "multiboot2.h"

static volatile unsigned char *video = (unsigned char *)0xB8000;

#define PORT 0x3f8 /* COM1 */

int inb(int port);

void outb(int reg, int port);

void init_serial() {
  outb(0x00, PORT + 1); // Disable all interrupts
  outb(0x80, PORT + 3); // Enable DLAB (set baud rate divisor)
  outb(0x03, PORT + 0); // Set divisor to 3 (lo byte) 38400 baud
  outb(0x00, PORT + 1); //                  (hi byte)
  outb(0x03, PORT + 3); // 8 bits, no parity, one stop bit
  outb(0xC7, PORT + 2); // Enable FIFO, clear them, with 14-byte threshold
  outb(0x0B, PORT + 4); // IRQs enabled, RTS/DSR set
}

int serial_received() { return inb(PORT + 5) & 1; }

char read_serial() {
  while (serial_received() == 0)
    ;

  return inb(PORT);
}

int is_transmit_empty() { return inb(PORT + 5) & 0x20; }

void write_serial(char a) {
  while (is_transmit_empty() == 0)
    ;

  outb(a, PORT);
}

static void clear(void) {
  for (int i = 0; i < 80 * 24 * 2; i++)
    video[i] = 0;
}

static void putchar(int c) {
  const int columns = 80;
  const int lines = 24;
  static int x = 0, y = 0;

  if (c == '\n' || x >= columns)
    x = 0, y++;

  if (c == '\n')
    return;

  if (y >= lines)
    y = 0;

  int idx = (x + y * columns) * 2;
  video[idx] = c & 0xff;
  video[idx + 1] = 7; /* vga attribute */

  ++x;
}

void puts(const char *str) {
  do
    putchar(*str);
  while (*str++);
  putchar('\n');
}

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
  puts("Write something to conemu console, it is then read by inb() function "
       "and after that wrote by outb function to conemu console");
  init_serial();
  while (1) {
    write_serial(read_serial());
  }
}
