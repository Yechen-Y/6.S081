#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *name)
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

  switch (st.type) {
    case T_FILE:
        fprintf(2, "Wrong path, the path is a file");
        return;
        break;
    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("ls: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0) //这个很关键啊 不然会出现无限///的情况
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0; // 此时buf 为 Path/filename
            if (strcmp(p, name) == 0) {
                printf("%s\n", buf);
                continue;
            }
            if (stat(buf, &st) < 0) {
                fprintf(2, "find: cannot stat %s\n", buf);
                continue;
            }
            if (st.type == T_DIR && (strcmp(p, ".") != 0) && (strcmp(p, "..") != 0)){
                find(buf, name);
            } 
        }
  }
  close(fd);
  return;
}


int main(int argc, char*argv[])
{
    if (argc < 3) {
        printf("Usage find [Path] [Filename]\n");
    } else if (argc > 3) {
        printf("Too much parameters\n");
    } else {
        find(argv[1], argv[2]);
    }

    exit(0);
}