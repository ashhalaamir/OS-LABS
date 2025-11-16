#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]){

	int t = uptime();
	printf("%d ticks since boot\n",t);
	exit(0);

}
