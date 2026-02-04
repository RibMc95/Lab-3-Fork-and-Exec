#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(void)
{
    int sleep_times[] = {1, 2, 3};
    pid_t pids[3];

    // Spawn 3 children with different sleep times
    for (int i = 0; i < 3; i++)
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
            printf("Child %d (PID %ld) sleeping for %d seconds\n", i, (long)getpid(), sleep_times[i]);
            sleep(sleep_times[i]);
            printf("Child %d (PID %ld) waking up\n", i, (long)getpid());
            _exit(0);
        }

        // Parent stores child PID
        pids[i] = pid;
    }

    // Parent polls with WNOHANG
    int children_reaped = 0;
    while (children_reaped < 3)
    {
        pid_t wpid = waitpid(-1, NULL, WNOHANG);
        if (wpid > 0)
        {
            printf("Parent: child %ld finished\n", (long)wpid);
            children_reaped++;
        }
        else if (wpid == 0)
        {
            // No child has finished yet, sleep briefly and try again
            usleep(100000); // 100ms
        }
        else
        {
            // Error
            perror("waitpid");
            return 1;
        }
    }

    printf("Parent: all children finished\n");
    return 0;
}
