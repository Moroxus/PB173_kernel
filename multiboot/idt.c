#include "idt.h"

uint64_t IDT[IDT_SIZE] = {0};

uint64_t create_idt_entry(uint32_t address)
{
    uint64_t descriptor = address >> 16;    // set higher part of the offset
    descriptor <<= 48;
    descriptor |= ((uint64_t)1 << 47);      // set present to 1
    descriptor |= ((uint64_t)3 << 45);      // set dpl to 3

    descriptor |= ((uint64_t)14 << 40);     // set 32-bit interrupt gate
    descriptor |= (8 << 16);               // set selector to CSS in GDT
    descriptor |= (address & 0x0000FFFF);   // set lower part of the offset

    return descriptor;
}
