#include <lib.h>

int main(int argc, char **argv) {
    char *filename = argv[1];
    int fd;
    if ((fd = open(filename, O_RDONLY)) >= 0) {
        close(fd);
        return 0;
    }
    fd = open(filename, O_CREAT);
    if (fd == -10) {
        printf("touch: cannot touch '%s': No such file or directory\n", filename);
        return 1;
    } else if (fd >= 0) {
        close(fd);
    }
    return 0;
}
