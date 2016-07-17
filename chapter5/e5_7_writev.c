#include <tlpi_hdr.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

struct iovec_p {
    void *base;
    size_t len;
};

ssize_t writev_p(int fd, struct iovec_p *iov, size_t iov_len)
{
    ssize_t bytes_written, total_bytes_written;
    size_t i;

    total_bytes_written = 0;
    for (i = 0; i < iov_len; i++) {
        bytes_written = write(fd, iov[i].base, iov[i].len);

        if (bytes_written == -1)
            return -1;

        total_bytes_written += bytes_written;
    }

    return total_bytes_written;
}

int main(int argc, char *argv[])
{
#define IOV_LEN 3
    int fd;
    ssize_t total_bytes_written;
    struct iovec_p iov[IOV_LEN];
    char c = '$';
    char *string_literal = "Rust is great!";
    char *dynamic_string;
    
    if (argc < 2)
        usageErr("%s filename\n", argv[0]);

    dynamic_string = (char *) malloc(19);
    if (dynamic_string == NULL)
        errExit("malloc dynamic_string");

    strncpy(dynamic_string, "Programming is fun\n", 19);

    iov[0].base = &c;
    iov[0].len = 1;

    iov[1].base = string_literal;
    iov[1].len = strlen(string_literal);

    iov[2].base = dynamic_string;
    iov[2].len = 19;
    
    fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1)
        errExit("open");

    total_bytes_written = writev_p(fd, iov, IOV_LEN);

    printf("total_bytes_written: %ld\n", (long) total_bytes_written);

    free(iov[2].base);
}

