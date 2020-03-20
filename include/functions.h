#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

/*The child processes signal handler*/
void child_sh(int signal_n)
{
    printf("\t[X] PROCESS CHILD: signal (SIGINT) received.\n");
    exit(EXIT_SUCCESS);
}

/*Install child's signal handler*/
void child_install_sh(pid_t pid)
{
    if (signal(SIGINT, child_sh) == SIG_ERR)
    {
        fprintf(stderr, "\t[X] PROCESS CHILD[%d]: Error installing signal handler.\n", pid);
        exit(EXIT_FAILURE);
    }
}

/*Open file*/
FILE *open_file(const char * path, const char *mode, pid_t pid)
{
    FILE *file;
    if ((file = fopen(path, mode)) == NULL)
    {
        fprintf(stderr, "[X] PROCESS[%d]: Error opening file.\n", pid);        
        exit(EXIT_FAILURE);
    }
    return file;
}

/*File writer*/
void my_write(const char *path, const char *mode, pid_t pid, char *content)
{
    FILE *file = open_file(path, mode, pid);
    fprintf(file, "%s", content);
    fclose(file);
}
