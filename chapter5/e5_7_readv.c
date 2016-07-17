#include <tlpi_hdr.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

struct iovec_p {
    void *base;
    size_t len;
};

ssize_t readv_p(int fd, struct iovec_p *iov, size_t iov_len)
{
    ssize_t bytes_read, total_bytes_read;
    size_t i;

    total_bytes_read = 0;
    for (i = 0; i < iov_len; i++) {
	bytes_read = read(fd, iov[i].base, iov[i].len);

	if (bytes_read == -1)
	    return -1;
	else if (bytes_read == 0)
	    break;
	
	total_bytes_read += bytes_read;
    }

    return total_bytes_read;
}

int main(int argc, char *argv[])
{
#define IOV_LEN 3
    int fd;
    char c;
    char buf[11];
    struct iovec_p iov[IOV_LEN];
    ssize_t total_bytes_read;

    iov[0].base = malloc(20);
    iov[0].len = 20;
    if (iov[0].base == NULL)
	errExit("malloc first iov buffer");

    iov[1].base = (void *) &c;
    iov[1].len = 1;

    iov[2].base = (void *) &buf;
    iov[2].len = 11;
    
    if (argc < 2)
	usageErr("%s filename\n", argv[0]);

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
	errExit("open");

    total_bytes_read = readv_p(fd, iov, IOV_LEN);
    if (total_bytes_read == -1)
	errExit("readv_p");

    printf("total_bytes_read: %ld\n", (long) total_bytes_read);
    
    free(iov[0].base);
}

