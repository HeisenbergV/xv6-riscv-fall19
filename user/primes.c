#include "kernel/types.h"
#include "user/user.h"

int 
is_primes_num(int number){
    int i, flag = 0;
    for(i=2; i<=number/2; ++i){
        if(number%i==0){
            flag=1;
            break;
        }
    }
    //flag==0 is primes number
    return flag;
}

int 
main(int argc, char *argv[]){
    int p[2];
    pipe(p);

    int number;

    if (fork() == 0){
        while(read(p[0], &number, 1)!=-1){
            if(is_primes_num(number))
                fprintf(2, "prime %d \n", number);
        }
    }
    else{
        for (int i = 2; i < 35; i++){
            write(p[1], &i, 1);
        }
    }
    exit();
}