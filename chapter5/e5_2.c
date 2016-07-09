#include <tlpi_hdr.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int fd;
    char *output = "probably appears at the end of the file anyway ...\n";

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
	usageErr("%s pathname\n");

    fd = open(argv[1], O_WRONLY | O_APPEND);
    if (fd == -1)
	errExit("open");

    if (lseek(fd, 0, SEEK_SET) == -1)
	errExit("lseek");

    if (write(fd, output, strlen(output)) == -1)
	errExit("write");

    if (close(fd) == -1)
	errExit("close");
}

