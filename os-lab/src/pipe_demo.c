#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int fd[2];
    if (pipe(fd) == -1)
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

    if(pid == 0)
    {
        // child: read from pipe and runn wc -l on stdin
        close(fd[1]);
        execlp("wc", "wc", "-l", (char *)NULL);
        perror("execlp");
        _exit(1);
    } else {
        //parent: write to pipe
        close(fd[0]);
        FILE *out = fdopen(fd[1], "w");
        for (int i = 0; i < 5; i++)
        {
            fprintf(out, "line %d\n", i);
        }
        fclose(out);
        waitpid(pid, NULL, 0);
    }
    return 0;
}



