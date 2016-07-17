///
/// <EOF> is a single bytes which denotes the end of file
///

fd1 = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
fd2 = dup(fd1);
fd3 = open(file, O_RDWR);
write(fd1, "Hello,", 6); // contents: "Hello,<EOF>"
write(fd2, " world", 6); // contents: "Hello, world<EOF>", because of dup fd1 and fd2 share the file offset
lseek(fd2, 0, SEEK_SET); // contents: "Hello, world<EOF>", change the file offset for fd2 AND fd1
write(fd1, "HELLO,", 6); // contents: "HELLO, world<EOF>", fd1 and fd2 share the offset
write(fd3, "Gidday", 6); // contents: "Gidday world<EOF>", fd3 refers to a different open file description
                         // and starts off at offset 0

