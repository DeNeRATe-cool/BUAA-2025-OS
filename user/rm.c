#include <lib.h>

int main(int argc, char **argv) {
    char *path;
    int flag_r = 0, flag_f = 0;
    if (argc == 2) {
        path = argv[1];
    } else {
        path = argv[2];
        if (strcmp("-r", argv[1]) == 0) {
            flag_r = 1;
        } else {
            flag_r = 1, flag_f = 1;
        }
    }
    int fd;
    if ((fd = open(path, O_RDONLY)) < 0) {
        if (!flag_f) {
            syscall_set_value(1);
            printf("rm: cannot remove '%s': No such file or directory\n", path);
            return 1;
        }
        syscall_set_value(0);
        return 0;
    }
    close(fd);
    struct Stat st;
    stat(path, &st);
    if (st.st_isdir && !flag_r) {
        syscall_set_value(1);
        printf("rm: cannot remove '%s': Is a directory\n", path);
        return 1;
    }
    remove(path);
    syscall_set_value(0);
    return 0;
}
