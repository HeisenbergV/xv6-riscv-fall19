#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    int parent_fd[2],child_fd[2];

    //创建管道 fd[0]读 fd[1]写
    pipe(parent_fd); 
    pipe(child_fd); 

    char msg = '0';
    if(fork() == 0) {//child 进程
        read(parent_fd[0], &msg, 1);//读取
        fprintf(2, "%d: child received ping, msg: %c \n", getpid(), msg);
        write(child_fd[1], &msg, 1);//写入
    } else{ //parent 进程
        write(parent_fd[1], &msg, 1);
        read(child_fd[0], &msg, 1);
        fprintf(2, "%d: parent received pong, msg: %c \n", getpid(), msg);
    }
    exit();
}
