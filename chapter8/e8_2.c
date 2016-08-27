#include <pwd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <tlpi_hdr.h>

/**
 * This function is obviously not thread-safe
 */
struct passwd *getpwnam_p(const char *name)
{
    struct passwd *pwd;

    // ensure we're starting from the beginning ...
    setpwent();

    while ((pwd = getpwent()) != NULL
	   && strcmp(pwd->pw_name, name) != 0)
	;

    endpwent();
    
    if (pwd == NULL)
	errno = ENOENT;
    
    return pwd;
}

int main(int argc, char *argv[])
{
    struct passwd *pwd;
    
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
	usageErr("%s <username>\n", argv[0]);

    pwd = getpwnam_p(argv[1]);

    if (pwd == NULL) {
	if (errno == ENOENT)
	    errExit("No matching entry found for \"%s\"", argv[1]);
	else
	    errExit("Unknown error occured");
    }

    printf("username : %-30s\n", pwd->pw_name);
    printf("password : %-30s\n", pwd->pw_passwd);
    printf("uid      : %-10ld\n", (long) pwd->pw_uid);
    printf("gid      : %-10ld\n", (long) pwd->pw_gid);
    printf("info     : %-30s\n", pwd->pw_gecos);
    printf("home     : %-20s\n", pwd->pw_dir);
    printf("shell    : %-20s\n", pwd->pw_shell);
}

