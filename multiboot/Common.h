#ifndef COMMON
#define COMMON

#include <stdlib.h>

extern volatile unsigned char *video;

void clear(void);

void vga_putchar(int c);

void vga_puts(const char *str);

void putDecimalDigit(unsigned int digit);

void putDecimalNumber(int number);

void putUnsignedDecimalNumber(unsigned int number);

unsigned int max(unsigned int first, unsigned int second);

#endif // COMMON
