#ifndef KERNEL_SYSCALL_H
#define KERNEL_SYSCALL_H

int serial_read(void* buff, int count);

int serial_write(void* buff, int count);

int do_syscall_in_C(int sys_number, void *buff, int count);

void Int128Handler();
void Int129Handler();

#endif // KERNEL_SYSCALL_H
