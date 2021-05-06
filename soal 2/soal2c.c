#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int first_pipe[2], second_pipe[2];

void execPs()
{
    dup2(first_pipe[1], 1);

    close(first_pipe[0]);
    close(first_pipe[1]);

    char *argv[] = {"ps", "aux", NULL};
    execv("/bin/ps", argv);
}

void execSort()
{
    dup2(first_pipe[0], 0);
    dup2(second_pipe[1], 1);

    close(first_pipe[0]);
    close(first_pipe[1]);
    close(second_pipe[0]);
    close(second_pipe[1]);

    char *argv[] = {"sort", "-nrk", "3,3", NULL};
    execv("/usr/bin/sort", argv);
}

void execHead()
{
    dup2(second_pipe[0], 0);

    close(second_pipe[0]);
    close(second_pipe[1]);

    char *argv[] = {"head", "-5", NULL};
    execv("/usr/bin/head", argv);
}

int main()
{
    if (pipe(first_pipe) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }

    if (fork() == 0) // ls
    {
        execPs();
    }

    if (pipe(second_pipe) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }

    if (fork() == 0) // wc -l
    {
        execSort();
    }

    close(first_pipe[0]);
    close(first_pipe[1]);

    if (fork() == 0) // wc -l
    {
        execHead();
    }
}