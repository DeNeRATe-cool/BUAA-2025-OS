#include <lib.h>

int main(int argc, char **argv) {
    int fd;
    if (argc == 2) {
        char *dir;
        dir = argv[1];
        if ((fd = open(dir, O_RDONLY)) >= 0) {
            close(fd);
            syscall_set_value(1);
            printf("mkdir: cannot create directory '%s': File exists\n", dir);
            return 1;
        }
        fd = open(dir, O_MKDIR);
        if (fd < 0) {
            syscall_set_value(1);
            printf("mkdir: cannot create directory '%s': No such file or directory\n", dir);
            return 1;
        }
        close(fd);
        syscall_set_value(0);
        debugf("created %s\n", dir);
        return 0;
    } else {
        char *dir = argv[2];
        if ((fd = open(dir, O_RDONLY)) >= 0) {
            close(fd);
            syscall_set_value(0);
            return 0;
        }
        char buf[MAXPATHLEN] = "";
        int i;
        int len = strlen(dir);
        for (i = 0; i < len; i++) {
            if (dir[i] == '/') {
                buf[i] = 0;
                if ((fd = open(buf, O_RDONLY)) >= 0) {
                    close(fd);
                } else {
                    break;
                }
            }
            buf[i] = dir[i];
        }
        for (; i < len; i++) {
            if (dir[i] == '/') {
                buf[i] = 0;
                fd = open(buf, O_MKDIR);
                if (fd >= 0) {
                    close(fd);
                }
            }
            buf[i] = dir[i];
        }
        buf[i] = 0;
        fd = open(buf, O_MKDIR);
        if (fd >= 0) {
            close(fd);
        }
        syscall_set_value(0);
        return 0;
    }
}
