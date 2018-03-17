#ifndef SERIAL_PORT
#define SERIAL_PORT

#define PORT 0x3f8 /* COM1 */

int inb(int port);

void outb(int reg, int port);

void init_serial();

int serial_received();

char read_serial();

int is_transmit_empty();

void write_serial(char a);

#endif // SERIAL_PORT
