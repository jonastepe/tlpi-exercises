#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

extern char **environ;

#define NAME_INVALID(name) ((name == NULL || strlen(name) == 0 || strstr(name, "=") != NULL) ? true : false)

int setenv_p(const char *name, const char *value, int overwrite)
{
    char *envar, *newvar;
    size_t len;

    if (NAME_INVALID(name)) {
	errno = EINVAL;
	return -1;
    }

    envar = getenv(name);
    
    // do nothing in case of an existing value and no overwrite
    if (envar != NULL && !overwrite)
	return 0;

    // name + '=' + 'value' + '\0'
    len = strlen(name) + strlen(value) + 2;

    newvar = malloc(len);
    if (newvar == NULL)
	return -1; // ENOMEM is the only failure reason for malloc, thus I need not set errno

    snprintf(newvar, len, "%s=%s", name, value);

    if (putenv(newvar) != 0)
	return -1; // Again, ENOMEM is the only failure reason for putenv
    else
	return 0;
}

void test_setenv_p()
{
    int ret;
    
    ret = setenv_p(NULL, "Idris", 0);
    assert(errno == EINVAL);
    assert(ret == -1);

    ret = setenv_p("", "Python", 0);
    assert(errno == EINVAL);
    assert(ret == -1);

    ret = setenv_p("Supi=", "Haskell", 0);
    assert(errno == EINVAL);
    assert(ret == -1);

    putenv("PROGLANG=C");
    ret = setenv_p("PROGLANG", "Rust", 0);
    assert(ret == 0);
    assert(strcmp(getenv("PROGLANG"), "C") == 0);

    ret = setenv_p("PROGLANG", "Rust", 1);
    assert(ret == 0);
    assert(strcmp(getenv("PROGLANG"), "Rust") == 0);

    ret = setenv_p("STILLGREAT", "C", 0);
    assert(ret == 0);
    assert(strcmp(getenv("STILLGREAT"), "C") == 0);
}

char **search_environ(const char *name)
{
    char **ep;

    for (ep = environ; ep != NULL; ep++)
        if (strstr(*ep, name) != NULL)
            break;

    return ep;
}

int unsetenv_p(const char *name)
{
    char **envar;
    
    if (NAME_INVALID(name)) {
        errno = EINVAL;
        return -1;
    }

    envar = search_environ(name);
    if (envar != NULL)
        printf("%s\n", *envar);
    
    return 0;
}

void test_unsetenv_p()
{
    int ret;
    putenv("REMOVE_THIS=JAVA");
    
    ret = unsetenv_p(NULL);
    assert(errno == EINVAL);
    assert(ret == -1);
    assert(getenv("REMOVE_THIS") != NULL);

    ret = unsetenv_p("");
    assert(errno == EINVAL);
    assert(ret == -1);
    assert(getenv("REMOVE_THIS") != NULL);

    ret = unsetenv_p("REMOVE_THIS=");
    assert(errno == EINVAL);
    assert(ret == -1);
    assert(getenv("REMOVE_THIS") != NULL);

    ret = unsetenv_p("REMOVE_THIS");
}

int main()
{
    test_setenv_p();
    test_unsetenv_p();
}

