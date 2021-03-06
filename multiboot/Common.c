#include "Common.h"

volatile unsigned char *video = (unsigned char *)0xB8000;

void clear(void) {
  for (int i = 0; i < 80 * 24 * 2; i++)
    video[i] = 0;
}

void vga_putchar(int c) {
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

void vga_puts(const char *str) {
  do
    vga_putchar(*str);
  while (*str++);
  vga_putchar('\n');
}


void putDecimalDigit(unsigned int digit) {
  switch (digit) {
    case 0: vga_putchar('0'); break;
    case 1: vga_putchar('1'); break;
    case 2: vga_putchar('2'); break;
    case 3: vga_putchar('3'); break;
    case 4: vga_putchar('4'); break;
    case 5: vga_putchar('5'); break;
    case 6: vga_putchar('6'); break;
    case 7: vga_putchar('7'); break;
    case 8: vga_putchar('8'); break;
    case 9: vga_putchar('9'); break;
    default: vga_putchar('e');
  }
}

void putDecimalNumber(int number) {
  if (number < 0) {
    vga_putchar('-');
    number = -number;
  }
  int counter = 1;
  int tmp = number;
  while ((tmp = tmp / 10) > 0) {
    ++counter;
  }
  tmp = number;
  unsigned int digits[counter];
  for (int i = 0; i < counter; ++i) {
    digits[i] = tmp % 10;
    tmp = tmp / 10;
  }
  for (int i = counter - 1; i >= 0; --i) {
    putDecimalDigit(digits[i]);
  }
}

void putUnsignedDecimalNumber(unsigned int number) {
  int counter = 1;
  unsigned int tmp = number;
  while ((tmp = tmp / 10) > 0) {
    ++counter;
  }
  tmp = number;
  unsigned int digits[counter];
  for (int i = 0; i < counter; ++i) {
    digits[i] = tmp % 10;
    tmp = tmp / 10;
  }
  for (int i = counter - 1; i >= 0; --i) {
    putDecimalDigit(digits[i]);
  }
}

unsigned int max(unsigned int first, unsigned int second) {
  if (first > second) {
    return first;
  }
  return second;
}

void* memcpy(void *dest, const void *src, unsigned int count) {
  const char *from = (const char *)src;
  char *to = (char *)dest;
  for (unsigned int i = 0; i < count; ++i) {
    *to = *from;
    ++to;
    ++from;
  }
  return dest;
}
