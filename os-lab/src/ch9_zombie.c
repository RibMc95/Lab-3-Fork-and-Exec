#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    printf("Parent PID: %ld\n", (long)getpid());

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        // Child exits immediately
        printf("Child PID: %ld, exiting immediately\n", (long)getpid());
        _exit(0);
    }

    // Parent sleeps 10 seconds - during this time, child is a zombie
    printf("Parent: child PID is %ld\n", (long)pid);
    printf("Parent: sleeping 10 seconds. Run 'ps aux' in another terminal to see zombie state.\n");
    sleep(10);

    // Now wait for the child (reap it)
    int status;
    if (waitpid(pid, &status, 0) < 0)
    {
        perror("waitpid");
        return 1;
    }

    printf("Parent: child reaped\n");
    return 0;
}
