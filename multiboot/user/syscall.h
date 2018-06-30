#ifndef SYSCALL_H
#define SYSCALL_H

int syscall(int sys_number, void *buff, int count);
void end(int);

#endif // SYSCALL_H
