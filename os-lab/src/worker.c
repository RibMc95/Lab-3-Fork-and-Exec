#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("worker: argc=%d\n", argc);
    for (int i = 0; i < argc; i++)
    {
        printf("  argv[%d]=%s\n", i, argv[i]);
    }

    const char *myvar = getenv("MYVAR");
    if (myvar)
    {
        printf("MYVAR=%s\n", myvar);
    }
    else
    {
        printf("MYVAR not set\n");
    }

    return 0;
}
