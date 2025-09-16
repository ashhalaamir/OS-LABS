#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *path, char *pattern, int do_exec, char *cmd)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if(st.type == T_DIR){
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      close(fd);
      return;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0) continue;
      if(!strcmp(de.name, ".") || !strcmp(de.name, "..")) continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      find(buf, pattern, do_exec, cmd);
    }
  } else {
    // check if filename matches the pattern
    char *fname = path + strlen(path) - strlen(pattern);
    if(fname >= path && strcmp(fname, pattern) == 0){
      if(do_exec){
        if(fork() == 0){
          char *argv[] = {cmd, path, 0};
          exec(cmd, argv);
          fprintf(2, "exec %s failed\n", cmd);
          exit(1);
        } else {
          wait(0);
        }
      } else {
        printf("%s\n", path);
      }
    }
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc < 3){
    fprintf(2, "usage: find dir pattern [-exec cmd]\n");
    exit(1);
  }

  int do_exec = 0;
  char *cmd = 0;

  if(argc == 5 && strcmp(argv[3], "-exec") == 0){
    do_exec = 1;
    cmd = argv[4];
  }

  find(argv[1], argv[2], do_exec, cmd);
  exit(0);
}

