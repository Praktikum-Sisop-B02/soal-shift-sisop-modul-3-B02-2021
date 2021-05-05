#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int fd1[2];
int fd2[2];

void execPS()
{
    dup2(fd1[1], STDOUT_FILENO);
    close(fd1[0]);
    close(fd1[1]);
    char *argv[] = {"ps", "-aux", NULL};
    execv("/bin/ps", argv);
}

void execSort()
{
    dup2(fd1[0], STDIN_FILENO);
    dup2(fd2[1], STDOUT_FILENO);

    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);
    char *argv[] = {"tail", "-10", NULL};
    execv("/usr/bin/tail", argv);
}

void execHead()
{
    dup2(fd2[0], STDIN_FILENO);

    close(fd2[0]);
    close(fd2[1]);

    char *argv[] = {"head", "-5", NULL};
    execv("/usr/bin/head", argv);
}

int main()
{
    if (pipe(fd1) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }
    if (pipe(fd2) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }

    if (fork() == 0)
    {
        execPS();
    }

    if (fork() == 0)
    {
        execSort();
    }

    execHead();
}