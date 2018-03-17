#include "serialPort.h"

void init_serial() {
  outb(0x00, PORT + 1); // Disable all interrupts
  outb(0x80, PORT + 3); // Enable DLAB (set baud rate divisor)
  outb(0x03, PORT + 0); // Set divisor to 3 (lo byte) 38400 baud
  outb(0x00, PORT + 1); //                  (hi byte)
  outb(0x03, PORT + 3); // 8 bits, no parity, one stop bit
  outb(0xC7, PORT + 2); // Enable FIFO, clear them, with 14-byte threshold
  outb(0x0B, PORT + 4); // IRQs enabled, RTS/DSR set
}

int serial_received() {
  return inb(PORT + 5) & 1;
}

char read_serial() {
  while (serial_received() == 0);
  return inb(PORT);
}

int is_transmit_empty() {
  return inb(PORT + 5) & 0x20;
}

void write_serial(char a) {
  while (is_transmit_empty() == 0);
  outb(a, PORT);
}
