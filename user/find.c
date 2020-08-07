#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *path, char *name) {
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

    if(read(fd, &de, sizeof(de) < 0)){
        return;
    }
    
    if (de.name == name){
        return;
    }

    if (st.type==T_DIR){
        find(path+name, name);
        return;
    }
}

int
main(int argc, char *argv[]) {

    find(".");
    exit();
}
