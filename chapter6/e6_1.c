/**
 * With 'mbuf' this program contains a substantial amount of static uninitialized data.
 * However, the compiled size of this program is way smaller than that. The reason is that
 * the uninitialized data segment is not present in the executable file. It will be present
 * in the virtual memory of the running program, but since it's uninitialized data, only the
 * size of the segment is recorded in the actual file and will be created upon running the
 * program.
 */

#include <stdio.h>
#include <stdlib.h>

char globBuf[65536];

int primes[] = {2, 3, 5, 7};

static int square(int x)
{
    int result;

    result = x * x;
    return result;
}

static void doCalc(int val)
{
    printf("The square of %d is %d\n", val, square(val));

    if (val < 1000) {
	int t;

	t = val * val * val;
	printf("The cube of %d is %d\n", val, t);
    }
}

int main(int argc, char *argv[])
{
    static int key = 9973;
    static char mbuf[10240000];
    char *p;

    p = malloc(1024);

    doCalc(key);

    exit(EXIT_SUCCESS);
}
