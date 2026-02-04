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
        // Child: prepare environment with MYVAR=hello
        char *args[] = {"./worker", "arg1", "arg2", "arg3", NULL};
        char *env[] = {"MYVAR=hello", "PATH=/usr/local/bin:/usr/bin:/bin", NULL};

        // Use execle to set custom environment
        execle("./worker", "worker", "arg1", "arg2", "arg3", (char *)NULL, env);
        perror("execle");
        _exit(127);
    }

    // Parent waits for child
    int status;
    if (waitpid(pid, &status, 0) < 0)
    {
        perror("waitpid");
        return 1;
    }

    if (WIFEXITED(status))
    {
        printf("Worker exited with status %d\n", WEXITSTATUS(status));
    }

    return 0;
}
