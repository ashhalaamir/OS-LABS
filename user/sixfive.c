#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

char *seps = "-\r\t\n./,";

void process(char *filename){
	int fd = open(filename, O_RDONLY);
	if (fd <0) {
		fprintf(2,"sixfive: cannot open %s\n", filename);
		return;
	}
	char buf[1];
	char numbuf[32];
	int nlen = 0;

	while (read(fd, buf, 1) == 1){
		char c = buf[0];
		if (c >='0' && c <= '9'){
			
			if(nlen < sizeof(numbuf) - 1){
				numbuf[nlen++] = c;
			}
		}
		else if  (strchr(seps,c)){
			if (nlen > 0){
				numbuf[nlen] = '\0';
				int val = atoi(numbuf);
				if (val%5 == 0 || val%6 == 0){
					printf("%d\n", val);
				}
				nlen=0;
			}
		}
	}

	if (nlen > 0) {
		numbuf[nlen] = '\0';
		int val = atoi(numbuf);
		if (val%5 == 0 || val%6 ==0){
			printf("%d\n",val);
		}
	}

	close(fd);
}

int main(int argc, char *argv[]){

	if (argc < 2){
		fprintf(2,"usage: sixfive files...\n");
		exit(1);
	}

	for (int i =1; i<argc; i++){
		process(argv[i]);
	}
	exit(0);
}
