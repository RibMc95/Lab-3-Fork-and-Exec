#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <pattern>\n", argv[0]);
        return 1;
    }

    int fds[2];
    if (pipe(fds) < 0)
    {
        perror("pipe");
        return 1;
    }

    // First child: run ls
    pid_t pid1 = fork();
    if (pid1 < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid1 == 0)
    {
        // Child 1: redirect stdout to pipe write end
        close(fds[0]); // Close read end
        if (dup2(fds[1], STDOUT_FILENO) < 0)
        {
            perror("dup2");
            _exit(1);
        }
        close(fds[1]); // Close original write end after dup

        execlp("ls", "ls", (char *)NULL);
        perror("execlp ls");
        _exit(127);
    }

    // Second child: run grep
    pid_t pid2 = fork();
    if (pid2 < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid2 == 0)
    {
        // Child 2: redirect stdin from pipe read end
        close(fds[1]); // Close write end
        if (dup2(fds[0], STDIN_FILENO) < 0)
        {
            perror("dup2");
            _exit(1);
        }
        close(fds[0]); // Close original read end after dup

        execlp("grep", "grep", argv[1], (char *)NULL);
        perror("execlp grep");
        _exit(127);
    }

    // Parent: close both ends of pipe
    close(fds[0]);
    close(fds[1]);

    // Parent waits for both children
    if (waitpid(pid1, NULL, 0) < 0)
    {
        perror("waitpid");
        return 1;
    }

    if (waitpid(pid2, NULL, 0) < 0)
    {
        perror("waitpid");
        return 1;
    }

    return 0;
}
