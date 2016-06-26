#include <tlpi_hdr.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    const size_t BUFSIZE = 512;
    char buf[BUFSIZE];
    int opt, fd, oflags = O_WRONLY | O_CREAT;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    char *filename;
    ssize_t bytes_read, bytes_written;
    
    if (argc < 2 || argc > 3)
	usageErr("%s [-a] file\n", argv[0]);

    if ((opt = getopt(argc, argv, ":a")) != -1) {
	switch (opt) {
	case 'a':
	    filename = argv[2];
	    oflags |= O_APPEND;
	    break;
	default: usageErr("%s [-a] file\n", argv[0]);
	}
    } else {
	filename = argv[1];
	oflags |= O_TRUNC;
    }
    
    if ((fd = open(filename, oflags, mode)) == -1)
	errExit("open file");

    while ((bytes_read = read(STDIN_FILENO, (void *) buf, BUFSIZE)) != 0) {
	if (bytes_read == -1)
	    errExit("read stdin");
	
	if ((bytes_written = write(STDOUT_FILENO, (void *) buf, (size_t) bytes_read) != bytes_read))
	    errExit("write stdout");

	if ((bytes_written = write(fd, (void *) buf, (size_t) bytes_read)) != bytes_read)
	    errExit("write file");
    }

    close(fd);
}


