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
        //Child: replace with "ls -l" (search PATH)
        printf("[child] exec 'ls -l'\n");
        execlp("ls", "ls", "-l", (char *)NULL);
        // If exec returns, it failed
        perror("execlp");
        _exit(1);
    } else {
        waitpid(pid, NULL, 0);
        printf("[parent] child finished\n");
    }
    return 0;
}
