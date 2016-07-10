#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <tlpi_hdr.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

int dup_p(int oldfd)
{
    return fcntl(oldfd, F_DUPFD, 0);
}

int dup2_p(int oldfd, int newfd)
{
    if (oldfd == newfd) {
	if (fcntl(oldfd, F_GETFL) == -1) {
	    errno = 	errExit("open f1");

    dup = dup_p(fd);
    if (dup == -1)
	errExit("dup f1");

    if (write(fd, fdtext, strlen(fdtext)) != strlen(fdtext))
	errExit("write f1 original");
    if (write(dup, duptext, strlen(duptext)) != strlen(duptext))
	errExit("write f1 duplicate");

    if (close(fd) == -1)
	errExit("close f1 fd");
    if (close(dup) == -1)
	errExit("close f1 dup");
}

void dup2_p_test()
{
    int fd = 3, dup, tmp;
    char *fdtext = "This is from the original.\n";
    char *duptext = "This is form the duplicate.\n";

    dup = dup2_p(fd, fd + 1);
    assert(dup == -1 && errno == EBADF);

    fd = open("f2", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1)
	errExit("open f2");

    dup = dup2_p(fd, fd);
    assert(dup == fd);

    dup = open("f2_tmp", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (dup == -1)
	errExit("open f2_tmp");

    tmp = dup;
    dup = dup2_p(fd, dup);
    assert(dup == tmp);

    if (write(fd, fdtext, strlen(fdtext)) != strlen(fdtext))
	errExit("write f2 original");
    if (write(dup, duptext, strlen(duptext)) != strlen(duptext))
	errExit("write f2 duplicate");

    if (close(fd) == -1)
	errExit("close f2 fd");
    if (close(dup) == -1)
	errExit("close f2 dup");
}

int main()
{
    // dup_p_test();
    //dup2_p_test();
}

