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

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        // Child: read numbers and sum them
        close(fds[1]); // Close write end

        int sum = 0;
        int num;
        while (read(fds[0], &num, sizeof(int)) > 0)
        {
            sum += num;
        }
        close(fds[0]); // Close read end

        printf("Sum = %d\n", sum);
        _exit(0);
    }

    // Parent: write numbers 1..10
    close(fds[0]); // Close read end

    for (int i = 1; i <= 10; i++)
    {
        if (write(fds[1], &i, sizeof(int)) < 0)
        {
            perror("write");
            close(fds[1]);
            return 1;
        }
    }
    close(fds[1]); // Close write end to signal EOF to child

    if (waitpid(pid, NULL, 0) < 0)
    {
        perror("waitpid");
        return 1;
    }

    return 0;
}
