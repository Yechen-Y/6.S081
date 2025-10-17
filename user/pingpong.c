#include "kernel/types.h"
#include "user/user.h"

/*
 * version: 0
 * data: 2025-10-13
 * author: YeChen 
 */
int main(int argc, char *argv[])
{
    int parentToChild[2];
    int childToParent[2];
    char buf[20] = {0};
    int pid;

    pipe(parentToChild);
    pipe(childToParent);

    if (fork() == 0) {
        pid = getpid();
        // 关闭parentToChild 的写入端口
        close(parentToChild[1]);
        read(parentToChild[0], buf, 4);
        close(parentToChild[0]);
        printf("%d: received %s\n", pid, buf);
        // 关闭childToParent 的读入端口
        close(childToParent[0]);
        write(childToParent[1], "pong", 4);
        close(childToParent[1]);
    } else {
        pid = getpid();
        // 关闭parentToChild 的读入端口
        close(parentToChild[0]);
        write(parentToChild[1], "ping", 4);
        close(parentToChild[1]);
        // 关闭childToParent 的写入端口
        close(childToParent[1]);
        read(childToParent[0], buf, 4);
        close(childToParent[0]);
        printf("%d: received %s\n", pid, buf);
    }


    exit(0);
}