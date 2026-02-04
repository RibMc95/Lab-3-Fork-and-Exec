#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid_a = fork();
    if (pid_a < 0)
    {
        perror("fork");
        return 1;
    }
    if (pid_a == 0)
    {
        // Child A: execl
        execl("/bin/echo", "echo", "one", "two", (char *)NULL);
        perror("execl");
        _exit(127);
    }

    int status_a;
    if (waitpid(pid_a, &status_a, 0) < 0)
    {
        perror("waitpid");
        return 1;
    }

    pid_t pid_b = fork();
    if (pid_b < 0)
    {
        perror("fork");
        return 1;
    }
    if (pid_b == 0)
    {
        // Child B: execv
        char *argv[] = {"echo", "one", "two", NULL};
        execv("/bin/echo", argv);
        perror("execv");
        _exit(127);
    }

    int status_b;
    if (waitpid(pid_b, &status_b, 0) < 0)
    {
        perror("waitpid");
        return 1;
    }

    printf("Both children finished.\n");
    return 0;
}
