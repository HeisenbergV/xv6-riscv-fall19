#include "kernel/types.h"
#include "user/user.h"

int 
is_primes(int num){
    int a = 0;
    int num = 0;
    for (int i = 2; i <= num - 1; i++){
        if (num % i == 0)
            a++;
    }

    return a==0;
}

int 
main(int argc, char *argv[]){
    int p[2];
    pipe(p);

    int number;

    int cid = fork();
    if (cid == 0){
        for (int i = 0; i < 32; i++)
            read(p[0], &number, 1);
            if(is_primes(number)==0)
                fprintf("%d", number);
    }
    else{
        for (int i = 2; i < 32; i++)
            write(p[1], &i, 1);
    }
    exit();
}