#include <tlpi_hdr.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>

int main()
{
    int fd1, fd2;
    int flags1, flags2;
    off_t off1, off2;
    char *s = "write this\n";

    fd1 = open("f", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd1 == -1)
	errExit("open");

    fd2 = dup(fd1);
    if (fd2 == -1)
	errExit("dup");

    flags1 = fcntl(fd1, F_GETFL);
    if (flags1 == -1)
	errExit("F_GETFL fd1");

    flags2 = fcntl(fd2, F_GETFL);
    if (flags2 == -1)
	errExit("F_GETFL fd2");

    assert(flags1 == flags2);

    if (write(fd1, s, strlen(s)) != strlen(s))
	errExit("write fd1");
    off1 = lseek(fd1, 0, SEEK_CUR);
    if (off1 == -1)
	errExit("first lseek fd1");
    off2 = lseek(fd2, 0, SEEK_CUR);
    if (off2 == -1)
	errExit("first lseek fd2");

    assert(off1 == off2);
    
    if (write(fd2, s, strlen(s)) != strlen(s))
	errExit("write fd2");
    off1 = lseek(fd1, 0, SEEK_CUR);
    if (off1 == -1)
	errExit("second lseek fd1");
    off2 = lseek(fd2, 0, SEEK_CUR);
    if (off2 == -1)
	errExit("second lseek fd2");

    assert(off1 == off2);

    if (close(fd1) == -1)
	errExit("close fd1");
    if (close(fd2) == -1)
	errExit("close fd2");
}

