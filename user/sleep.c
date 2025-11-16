#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
if (argc<2) {
fprintf( 2, "Usage: sleep <ticks>\n");
exit(1);
}

  char *s =argv[1];
for (int i=0; s[i]; i++){
if (s[i] < '0' || s[i] > '9'){
fprintf(2, "sleep:argument must be a non-negative integer\n");
exit(1);
}
}
int n =atoi(argv[1]);
pause(n);
exit(0);
}
