#include "kernel_syscall.h"
#include "serialPort.h"

int serial_read(void* buff, int count) {
  char *cbuf = (char *)buff;
  for (int i = 0; i < count; ++i) {
    cbuf[i] = read_serial();
  }
  return count;
}

int serial_write(void* buff, int count) {
  const char *cbuf = (const char *)buff;
  for (int i = 0; i < count; ++i) {
    write_serial(cbuf[i]);
  }
  return count;
}

int do_syscall_in_C(int sys_number, void *buff, int count) {
  switch (sys_number) {
    case 1:
      return serial_read(buff, count);
      // return count;
    case 2:
      return serial_write(buff, count);
      // return count;
    default:
      return -1;
  }
}
