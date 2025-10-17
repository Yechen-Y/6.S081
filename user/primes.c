#include "kernel/types.h"
#include "user/user.h"

#define RD 0
#define WR 1
#define NUM 34

/*
* data: 2025-10-15
* author: Yechen
*/

const uint INT = sizeof(int);
void init_Buffer(int *buffer, int num);
void prime(int *p);

int main(int argc, char *argv[]) 
{
    int buffer[NUM];
    int p[2];
    pipe(p);
    if (fork() == 0) {
        prime(p);
    } else {
        init_Buffer(buffer, NUM);
        close(p[RD]);
        write(p[WR], buffer, NUM * INT);
        close(p[WR]);
        wait(0);
    }
    exit(0);
}

void init_Buffer(int *buffer, int num) 
{
    for (int count = 0; count < num; count++) {
        *buffer++ = count + 2;
    }
}

// prime() 从左读取数据，向右传输数据

void prime(int *p) 
{
    int firstNum;
    int pi[2];
    int data;

    // 读取第一个数据
    close(p[WR]);
    read(p[RD], &firstNum, INT);


    if (firstNum != 0) {
        printf("prime %d\n", firstNum); // 打印第一个素数
        pipe(pi);
        if (fork() == 0) {
            prime(pi);
        } else {
            //发送数据
            close(pi[RD]);
            while ((read(p[RD], &data, INT)) == INT) {
                if (data % firstNum) {
                    write(pi[WR], &data, INT);
                }
            }
            close(pi[WR]);
            close(p[RD]);
            wait(0);
            exit(0);
        }              
    }
    close(p[RD]);
    exit(0);

}
