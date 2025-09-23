#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int t = uptime();   // call system call
  printf("%d ticks\n", t);
  exit(0);
}

