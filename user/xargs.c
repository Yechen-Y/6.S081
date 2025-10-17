#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

// 从stdin 读取参数以\n分割
// argv的格式为 xagrs cmd cmd, argv[argc - 1]的第一个参数...
void myexec(char **var);
int readLine(char *line);
void splitLine(char *line, char *buf[]);
void generateVar(int argc, char *argv[], char *cmd[], char *line[]);

int main(int argc, char *argv[])
{   
    if (argc < 2) {
        printf("Please enter more parameters!\n");
        exit(1);
    }
    char *cmd[MAXARG];
    char line[1024]; //一行的内容
    char *buf[MAXARG];
    while(readLine(line)) {
        splitLine(line, buf);
        generateVar(argc, argv, cmd, buf);
        myexec(cmd);
    }
    
    exit(0);
}

// 从stdint 读取一个line
// 如果读取完之后没有\n 为结尾
int readLine(char *line)
{
    int end = 0;

    while (read(0, &line[end], 1)) {
        if (line[end] == '\n') {
            line[end] = '\0';
            return 1;
        } else {
            end++;
        }
    }
    if (end != 0){
        line[end] = '\0';
        return 1;
    }
    return 0;
}

// 将读取到的一行进行分割
void splitLine(char *line, char *buf[])
{
    char *start = line; // line的每个参数的起始点
    int i = 0; 

    while (*line) {
        if (*line == ' ') {
            *line = '\0';
            buf[i] = start;
            start = line + 1;
            i++;
        }
        line++;
    }
    // 添加最后一个参数
    buf[i] = start;

    buf[i + 1] = 0; // NULL
}


// 生成参数数组
void generateVar(int argc, char *argv[], char *cmd[], char *line[])
{
    int count = 0;
    // 从argv提取出cmd
    for (int i = 1; i < argc; i++) {
        cmd[i - 1] = argv[i]; 
    }
    //将两个字符数组合并
    while (line[count] != 0) {
        cmd[argc - 1 + count] = line[count];
        count++;
    }
    cmd[argc - 1 + count] = 0; // NULL
}

// 根据参数数组执行命令
void myexec(char **var)
{

    if (fork() == 0) {
        exec(var[0], var);
    } else {
        wait(0);
    }

}
