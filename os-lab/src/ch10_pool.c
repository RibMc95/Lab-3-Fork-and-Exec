#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <concurrency_limit> <task1> [task2] ...\n", argv[0]);
        return 1;
    }

    int max_concurrent = atoi(argv[1]);
    if (max_concurrent <= 0)
    {
        fprintf(stderr, "Concurrency limit must be positive\n");
        return 1;
    }

    int num_tasks = argc - 2;
    char **tasks = &argv[2];

    // Array to track active child PIDs
    pid_t *active_pids = malloc(max_concurrent * sizeof(pid_t));
    int active_count = 0;
    int next_task = 0;

    // Spawn initial batch of workers
    for (int i = 0; i < max_concurrent && next_task < num_tasks; i++)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return 1;
        }

        if (pid == 0)
        {
            // Child: process the task
            printf("[%ld] Starting task: %s\n", (long)getpid(), tasks[next_task]);
            sleep(1); // Simulate work
            printf("[%ld] Finished task: %s\n", (long)getpid(), tasks[next_task]);
            _exit(0);
        }

        // Parent: store child PID
        active_pids[active_count++] = pid;
        next_task++;
    }

    // Main loop: wait for children and spawn new ones
    while (next_task < num_tasks || active_count > 0)
    {
        pid_t wpid = waitpid(-1, NULL, WNOHANG);

        if (wpid > 0)
        {
            // A child finished, remove from active list
            for (int i = 0; i < active_count; i++)
            {
                if (active_pids[i] == wpid)
                {
                    active_pids[i] = active_pids[active_count - 1];
                    active_count--;
                    break;
                }
            }

            // Spawn a new child if there are remaining tasks
            if (next_task < num_tasks)
            {
                pid_t pid = fork();
                if (pid < 0)
                {
                    perror("fork");
                    return 1;
                }

                if (pid == 0)
                {
                    // Child: process the task
                    printf("[%ld] Starting task: %s\n", (long)getpid(), tasks[next_task]);
                    sleep(1); // Simulate work
                    printf("[%ld] Finished task: %s\n", (long)getpid(), tasks[next_task]);
                    _exit(0);
                }

                // Parent: store child PID
                active_pids[active_count++] = pid;
                next_task++;
            }
        }
        else if (wpid == 0)
        {
            // No child finished yet, sleep briefly and try again
            usleep(100000); // 100ms
        }
        else if (wpid < 0)
        {
            perror("waitpid");
            return 1;
        }
    }

    free(active_pids);
    printf("All tasks completed\n");
    return 0;
}
