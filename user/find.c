#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/param.h"

void find(char *path, char *target, int doexec, int cmdargc, char **cmdargv);

char* fmtname(char *path) {
  static char buf[DIRSIZ+1];
  char *p;
  for (p = path + strlen(path); p >= path && *p != '/'; p--);
  p++;
  if (strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  buf[strlen(p)] = 0;
  return buf;
}

void find(char *path, char *target, int doexec, int cmdargc, char **cmdargv){
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, O_RDONLY)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if (st.type == T_FILE) {
    if (strcmp(fmtname(path), target) == 0) {
      if (doexec) {
        char *nargv[MAXARG];
        for (int i = 0; i < cmdargc; i++) nargv[i] = cmdargv[i];
        nargv[cmdargc] = path;
        nargv[cmdargc+1] = 0;
        if (fork() == 0) {
          exec(nargv[0], nargv);
          fprintf(2, "exec %s failed\n", nargv[0]);
          exit(1);
        }
        wait(0);
      } else {
        printf("%s\n", path);
      }
    }
  }

  if (st.type == T_DIR) {
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
      printf("find: path too long\n");
    } else {
      strcpy(buf, path);
      p = buf + strlen(buf);
      *p++ = '/';
      while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0) continue;
        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        find(buf, target, doexec, cmdargc, cmdargv);
      }
    }
  }
  close(fd);
}

int main(int argc, char *argv[]){
  if (argc < 3) {
    fprintf(2, "Usage: find <dir> <filename> [-exec cmd]\n");
    exit(1);
  }

  if (argc > 3 && strcmp(argv[3], "-exec") == 0) {
    find(argv[1], argv[2], 1, argc-4, &argv[4]);
  } else {
    find(argv[1], argv[2], 0, 0, 0);
  }

  exit(0);
}
