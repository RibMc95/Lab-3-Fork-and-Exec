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

    if(pid == 0)
    {
        printf("[Child] PID: %d, PPID: %d\n", getpid(), getppid());
        sleep(1); //Show concurrency
        printf("[child] exiting with code 42\n");
        _exit(42); // use_exit in child after fork
    } else {
        // Parent 
        printf("[parent] created child PID: %d\n", pid);
        int status;
        pid_t wpid = waitpid(pid, &status, 0);
        if (wpid < 0)
        {
            perror("waitpid");
            return 1;
        }
        if (WIFEXITED(status))
        {
            printf("[parent] child exited with code %d\n", WEXITSTATUS(status));
        }
        else
        {
            printf("[parent] child did not exit normally\n");
        }
        return 0;
    }
}