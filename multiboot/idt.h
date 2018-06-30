#ifndef IDT_H

#include <stdint.h>

#define IDT_SIZE 256
extern uint64_t IDT[IDT_SIZE];

void setIdt(uint32_t ptr);

uint64_t create_idt_entry(uint32_t address);

#define IDT_H
#endif // IDT_H
