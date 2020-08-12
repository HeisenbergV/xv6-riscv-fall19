#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


char* 
fmtname(char *path) {
    char *p;
    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--);
    return ++p;
}
 
void
find(char*path, char*name) {
    int fd;
    struct stat st;

    if (strcmp(fmtname(path), name) == 0) {
        fprintf(2, "%s \n", path);
    }
    
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (st.type != T_DIR){
        close(fd);
        return;
    }

    char buf[512], *p;
    struct dirent de;

    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0)
            continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if (strcmp(de.name, ".") == 0)
            continue;
     
        if (strcmp(de.name, "..") == 0)
            continue;

        find(buf, name);
    }
}

int
main(int argc, char *argv[]) {
    if (argc <= 2) {
        fprintf(2, "params error");
        exit();
    }

    find(argv[1], argv[2]);
    exit();
}
