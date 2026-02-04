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
        // Child: execute ls -la
        execlp("ls", "ls", "-la", (char *)NULL);
        perror("execlp");
        _exit(127);
    }

    // Parent waits for child
    int status;
    if (waitpid(pid, &status, 0) < 0)
    {
        perror("waitpid");
        return 1;
    }

    printf("Child process finished\n");
    return 0;
}
