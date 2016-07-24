#include <stdio.h>
#include <setjmp.h>

void do_setjmp();
void do_longjmp();

jmp_buf env;

int main()
{
    printf("starting...\n");
    
    do_setjmp();
    do_longjmp();

    printf("finished...\n");
}

void do_setjmp()
{
    int value = 10;
    
    if (setjmp(env) == 0)
	printf("initial setjmp(): value is %d\n", value);
    else
	printf("returning from longjmp(): value is %d\n", value);
}

void do_longjmp()
{
    longjmp(env, 1);
}

