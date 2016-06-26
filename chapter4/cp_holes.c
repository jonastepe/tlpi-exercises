#include <tlpi_hdr.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSIZE ((size_t) 1024)

int main(int argc, char *argv[])
{
    int in_fd, out_fd;
    char buf[BUFSIZE];
    ssize_t num_read;
    
    if (argc != 3 || strcmp(argv[1], "--help") == 0)
	usageErr("%s src dest\n", argv[0]);

    in_fd = open(argv[1], O_RDONLY);
    if (in_fd == -1)
	errExit("open src file");

    out_fd = open(argv[2], O_WRONLY | O_CREAT,
		  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (out_fd == -1)
	errExit("open target file");

    while ((num_read = read(in_fd, buf, BUFSIZE)) > 0) {
	off_t i = 0, j;

	while (i < num_read) {
	    for (j = i; j < num_read && buf[j] != '\0'; j++);

	    size_t non_zero_bytes = (size_t) j - i;
	    if (write(out_fd, &buf[i], non_zero_bytes) != non_zero_bytes)
		errExit("write to dest file");

	    for (i = j; i < num_read && buf[i] == '\0'; i++);

	    off_t hole_length = i - j;
	    if (hole_length != 0) {
		if (lseek(out_fd, hole_length, SEEK_CUR) == -1)
		    errExit("lseek on dest file");
	    }
	}
    }

    if (num_read == -1)
	errExit("read from src file");

    if (close(in_fd) == -1)
	errExit("close src file");
    if (close(out_fd) == -1)
	errExit("close dest file");
}

