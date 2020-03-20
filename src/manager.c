#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <definitions.h>
#include <functions.h>

void signal_handler(int signal_number);
void install_signal_handler();
pid_t create_single_process(const char *exe, const char *class, const char *arg);
void wait_childs();
void create_pipe(int *fd);
void create_processes(const char *arg);
float read_mean(int read_fd);
void write_mean(float mean);
void finalize_processes();
pid_t execute_pd();

/*Declaration of global variables*/
pid_t pids[PROCESSES_NUMBER];


int main(int argc, char *argv[]){

    /*Pipe descriptors*/
    int fd[2];
    char write_fd[SIZE] = "";
    /*Mean mark of students*/
    float mean = 0;

    install_signal_handler();

    printf("\n[X] MANAGER[%d]: Starting...\n\n", getpid());

    my_write(LOG_PATH, WRITE_MOD, getpid(), "******** Log del sistema ********\n");

    /*Create pipe (for manager communicate with PD)*/
    create_pipe(fd);
    /*Copy write file descriptor for PC*/
    sprintf(write_fd, "%d", fd[WRITE_FD]);

    /*Create process (PA, PB and PC)*/
    create_processes(write_fd);

    /*Read and write mean mark*/
    mean = read_mean(fd[READ_FD]);

    /*Generate log.txt (files/log.txt)*/
    write_mean(mean);

    /*End message*/
    printf("----------------------------------------------------------------------------------------\n");
    printf("\t\t\t***** PROGRAM FINALIZED *****\n\n");

    return EXIT_SUCCESS;
}

void signal_handler(int signal_n)
{
    printf("\n---------------------------------------------------------\n");
    printf("[X] MANAGER[%d]: signal (SIGINT) received.\n", getpid());
    
    finalize_processes();
    
    /*Generate remove process*/
    pids[PROCESSES_NUMBER-1] = execute_pd();

    exit(EXIT_SUCCESS);
}

void install_signal_handler()
{
    if (signal(SIGINT, signal_handler) == SIG_ERR)
    {
        fprintf(stderr, "[X] MANAGER[%d]: Error installing signal handler.\n", getpid());
        exit(EXIT_FAILURE);
    }
}

void finalize_processes()
{
    int i;

    printf("\n[X] MANAGER[%d]: Terminating running child processes...\n", getpid());

    for (i = 0; i < PROCESSES_NUMBER-1; i++) {
    if(pids[i]!=ERROR){
      /*Send signal:*/
      if (kill(pids[i], SIGINT) == ERROR) {
        fprintf(stderr, "[X] MANAGER[%d]: Error using kill() on CHILD[%d].\n", getpid(), pids[i]);
      }
    }
  }
}

void wait_childs(){
    int i;
    for ( i = 1; i < PROCESSES_NUMBER-1; i++)
    {
        if((pids[i] = wait(NULL)) == ERROR){
            fprintf(stderr, "[X] MANAGER[%d]: Error waiting the child processes.\n", getpid());
            exit(EXIT_FAILURE);
        }
    }
}

/*Create a single process*/
pid_t create_single_process(const char *exe, const char *class, const char *arg){
    pid_t pid;
    switch (pid = fork())
    {
    case -1:
        fprintf(stderr, "[X] MANAGER[%d]: Error create CHILD[%d] process.\n", getppid(), pid);
        exit(EXIT_FAILURE);

    case 0:
        if (execl(exe, class, arg, NULL) == ERROR)
        {
            fprintf(stderr, "[X] MANAGER[%d]: Error execute CHILD[%d] process.\n", getppid(), pid);
            exit(EXIT_FAILURE);
        }
        break;       
    }

    return pid;
}

void create_pipe(int *fd){
    if(pipe(fd) == ERROR){
        fprintf(stderr, "[X] MANAGER[%d]: Error create pipe.\n", getppid());
        exit(EXIT_FAILURE);
    }
}

/*Create all process*/
void create_processes(const char *arg){
    int i;
    for (i = 0; i < PROCESSES_NUMBER-1; i++)
    {
        switch (i)
        {
            case 0:
                pids[i] = create_single_process(PA_EXEC, PA, NULL);
                /*Waiting pa finalize*/
                pids[i] = wait(NULL);
                break;
            case 1:
                pids[i] = create_single_process(PB_EXEC, PB, NULL);
                break;
            default:
                pids[i] = create_single_process(PC_EXEC, PC, arg);
                break;
        }
    }
    
    wait_childs(pids);
}

/*Read mean of marks*/
float read_mean(int read_fd){
    char msg[SIZE];

    if(read(read_fd, msg, sizeof(msg)) == ERROR){
        fprintf(stderr, "[X] MANAGER[%d]: Error readding from the pipe.\n", getpid());
        exit(EXIT_FAILURE);
    }

    return atof(msg);
}

/*Write mean of manrks in files/log.txt*/
void write_mean(float mean){
    char mean_l[MAX_SIZE] = "";
    sprintf(mean_l, "La nota media de la clase es: %.2f.\n", mean);
    my_write(LOG_PATH, APPEND_MOD,getpid(), mean_l);
    my_write(LOG_PATH, APPEND_MOD, getpid(), "FIN DE PROGRAMA\n");

    printf("\n[X] MANAGER[%d]: Generated 'files/log.txt' | The student's mean mark is %.2f.\n", getpid(), mean);
}

/*Execute pd process*/
pid_t execute_pd(){
        pid_t pid = create_single_process(PD_EXEC, PD, NULL);
        if((pid = wait(NULL)) == ERROR){
            fprintf(stderr, "[X] MANAGER[%d]: Error waiting PD[%d].\n", getppid(), pid);
            exit(EXIT_FAILURE);
        }
        
        return pid;
}
