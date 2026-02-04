#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s N\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0)
    {
        fprintf(stderr, "N must be a positive integer\n");
        return 1;
    }

    pid_t *pids = malloc(n * sizeof(pid_t));
    if (!pids)
    {
        perror("malloc");
        return 1;
    }

    // Create N children
    for (int i = 0; i < n; i++)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return 1;
        }

        if (pid == 0)
        {
            // Child process
            printf("Child %d (PID %ld) exiting with status %d\n", i, (long)getpid(), i + 1);
            exit(i + 1);
        }

        // Parent: store child PID
        pids[i] = pid;
    }

    // Parent waits for all children
    for (int i = 0; i < n; i++)
    {
        int status;
        pid_t wpid = waitpid(pids[i], &status, 0);
        if (wpid < 0)
        {
            perror("waitpid");
            return 1;
        }

        if (WIFEXITED(status))
        {
            int exit_code = WEXITSTATUS(status);
            printf("Child %ld exited with status %d\n", (long)wpid, exit_code);
        }
    }

    free(pids);
    return 0;
}
