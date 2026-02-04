#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int fds[2];
    if (pipe(fds) < 0)
    {
        perror("pipe");
        return 1;
    }

    pid_t left = fork();
    if (left < 0)
    {
        perror("fork");
        return 1;
    }

    if (left == 0)
    {
        if (dup2(fds[1], STDOUT_FILENO) < 0)
        {
            perror("dup2");
            _exit(1);
        }
        close(fds[0]);
        close(fds[1]);
        char *args[] = {"/bin/ls", "-1", NULL};
        execv(args[0], args);
        perror("execv");
        _exit(127);
    }

    pid_t right = fork();
    if (right < 0)
    {
        perror("fork");
        return 1;
    }

    if (right == 0)
    {
        if (dup2(fds[0], STDIN_FILENO) < 0)
        {
            perror("dup2");
            _exit(1);
        }
        close(fds[1]);
        close(fds[0]);
        char *args[] = {"/bin/grep", "demo", NULL};
        execv(args[0], args);
        perror("execv");
        _exit(127);
    }

    close(fds[0]);
    close(fds[1]);

    if (waitpid(left, NULL, 0) < 0)
    {
        perror("waitpid");
        return 1;
    }
    if (waitpid(right, NULL, 0) < 0)
    {
        perror("waitpid");
        return 1;
    }

    return 0;
}
