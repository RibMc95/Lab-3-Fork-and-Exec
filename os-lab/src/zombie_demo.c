#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        printf("Child exiting immediately (pid=%ld)\n", (long)getpid());
        _exit(0);
    }

    printf("Parent sleeping before wait (pid=%ld, child=%ld)\n", (long)getpid(), (long)pid);
    sleep(2);

    if (waitpid(pid, NULL, 0) < 0)
    {
        perror("waitpid");
        return 1;
    }

    printf("Parent reaped child\n");
    return 0;
}

/* How to observe the zombie process:
#Terminal 1
./os-lab/build/zombie_demo

#Terminal 2(during the 2 - second sleep)
                          ps aux |
    grep zombie_demo */