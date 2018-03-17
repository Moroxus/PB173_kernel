#ifndef COMMON
#define COMMON

#define NULL 0x0

extern volatile unsigned char *video;

void clear(void);

void putchar(int c);

void puts(const char *str);

void putDecimalDigit(unsigned int digit);

void putDecimalNumber(int number);

void putUnsignedDecimalNumber(unsigned int number);

unsigned int max(unsigned int first, unsigned int second);

void* memcpy(void *dest, const void *src, unsigned int count);

#endif // COMMON
