#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int isdelim(char c) {
  return (c == ' ' || c == '-' || c == '\r' || c == '\t' ||
          c == '\n' || c == '.' || c == '/' || c == ',');
}

int main(int argc, char *argv[]) {
  if(argc < 2){
    fprintf(2, "Usage: sixfive file...\n");
    exit(1);
  }

  for(int i = 1; i < argc; i++){
    int fd = open(argv[i], O_RDONLY);
    if(fd < 0){
      fprintf(2, "sixfive: cannot open %s\n", argv[i]);
      continue;
    }

    char buf[512];
    int n, j = 0;
    char num[64];

    while((n = read(fd, buf, sizeof(buf))) > 0){
      for(int k = 0; k < n; k++){
        char c = buf[k];
        if(c >= '0' && c <= '9'){
          if(j < sizeof(num)-1) num[j++] = c;
        } else {
          if(j > 0){
            num[j] = '\0';
            int val = atoi(num);
            if(val % 5 == 0 || val % 6 == 0)
              printf("%d\n", val);
            j = 0;
          }
          if(isdelim(c)){
            // just skip delimiter
          }
        }
      }
    }

    if(j > 0){ // flush last number if file doesn't end with a delimiter
      num[j] = '\0';
      int val = atoi(num);
      if(val % 5 == 0 || val % 6 == 0)
        printf("%d\n", val);
      j = 0;
    }

    close(fd);
  }

  exit(0);
}

