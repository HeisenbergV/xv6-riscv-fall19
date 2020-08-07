#include "kernel/types.h"
#include "user/user.h"


void
filter(int out, int input){
    close(input);
    int div;
    //1. 输出第一个
    if(read(out, &div, sizeof(int))<=0) exit();
    fprintf(2, "prime %d \n", div);

    int p[2];
    pipe(p);

    //3. 让子进程继续重复操作
    if(fork()){
        filter(p[0],p[1]);
        return;
    }

    //2. 过滤，符合要求的发送到新的pipe
    int num;
    while(read(out, &num, sizeof(int))){
        if (num % div != 0){
            write(p[1], &num, sizeof(int));
        }
    }
    close(out);

    wait();
    exit();
}

int 
main(int argc, char *argv[]){
    int p[2];
    pipe(p);

    if(fork()){
        filter(p[0], p[1]);
    }else{
        for (int i = 2; i <= 35; i++){
            write(p[1], &i, sizeof(i));
        }
        wait();
        exit();
    }
    return 0;
}