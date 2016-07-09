#include <tlpi_hdr.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    bool no_append = false;
    int num_bytes, fd, counter;
    int oflag = O_WRONLY | O_CREAT;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    char byte = '#';
    
    if (argc < 3 || strcmp(argv[1], "--help") == 0)
	usageErr("%s filename num-bytes [x]\n", argv[0]);

    num_bytes = getInt(argv[2], GN_ANY_BASE | GN_GT_0, "num-bytes");
    
    if (argc == 4 && strcmp(argv[3], "x") == 0)
	no_append = true;

    if (!no_append)
        oflag |= O_APPEND;
    
    fd = open(argv[1], oflag, mode);
    if (fd == -1)
        errExit("open");

    for (counter = 0; counter < num_bytes; counter++) {
	if (no_append)
	    if (lseek(fd, 0, SEEK_END) == -1)
		errExit("lseek");

	if ((write(fd, &byte, 1)) != 1)
	    errExit("write");
    }

    if (close(fd) == -1)
	errExit("close");
}

