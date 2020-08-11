#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"



char*
fmtname(char *path) {
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for (p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    return buf;
}


void
find(char*path, char*name) {
    int fd;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    char buf[512], *p;
    struct dirent de;

    if (st.type == T_FILE) {
        if (strcmp(fmtname(path), name)) {
            fprintf(2, "%s \n", path);
        }
        return;
    }

    if (st.type != T_DIR)
        return;

    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        return;

    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {

        if (de.inum == 0)
            continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if (strcmp(de.name, '.'))
            continue;

        if (strcmp(de.name, name))
            fprintf(2, "%s \n", buf);
        else
            find(buf, name);
    }

}

int
main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "not find file name");
        exit();
    }

    find(argv[0], argv[1]);
    exit();
}
