#include "kernel/types.h"
#include "user/user.h"



//每一个子进程的任务：
//1. 输出第一个数字； 
//2. fork新进程，进行下次过滤
//3. 将剩下的过滤发送到新的pipe；
//4. 关闭不用的fd；
void 
filter(int input){
    int num, div;
    if (read(input, &div, sizeof(num))<=0) exit();

    //1
    fprintf(2, "prime %d \n", div);
    int p[2];
    pipe(p);

    //2
    if (fork()){
        close(p[1]);
        filter(p[0]);
        return;
    }

    close(p[0]);

    //3
    while(read(input, &num, sizeof(num))){
        if (num % div != 0)
            write(p[1], &num, sizeof(num));
    }

    //4
    close(p[1]);
    close(input);
    wait();
    exit();
}

int 
main(int argc, char *argv[]){
    int p[2];
    pipe(p);

    if(fork()){
        filter(p[0]);
    }else{
        for (int i = 2; i <= 35; i++){
            write(p[1], &i, sizeof(i));
        }
    }
    exit();
}