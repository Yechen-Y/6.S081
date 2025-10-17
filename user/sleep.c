#include "kernel/types.h"
#include "user/user.h"

/*
* version: 0.0
* date: 2025-10-12 21:56
* author: YeChen
*/

int main(int argc, char *argv[])
{
    int state;
    if (argc <= 1) {
        fprintf(2, "Please enter a number.\n");
        exit(1);
    }
    if (argc > 2) {
        fprintf(2, "Enter too much number.\n");
        exit(1);
    }
    state = sleep(atoi(argv[1]));
    if (state) {
        fprintf(2, "Something wrong.\n");
        exit(1);
    } 
    exit(0);
}