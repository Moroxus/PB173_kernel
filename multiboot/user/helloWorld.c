#include <stdio.h>
#include "syscall.h"

int main();

void _start() {
  int retval = main();
  end(retval);
}

int main() {
  printf("Hello world in userland !!\n");
  return 7;
}
