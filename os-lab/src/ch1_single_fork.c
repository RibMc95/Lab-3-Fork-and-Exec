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
        // Child process
        printf("Hello from child (PID = %ld, PPID = %ld)\n", (long)getpid(), (long)getppid());
        sleep(2);
        exit(7);
    }

    // Parent process
    int status;
    if (waitpid(pid, &status, 0) < 0)
    {
        perror("waitpid");
        return 1;
    }

    if (WIFEXITED(status))
    {
        int exit_code = WEXITSTATUS(status);
        printf("child %ld exited with status %d\n", (long)pid, exit_code);
    }

    return 0;
}
