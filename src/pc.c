#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <definitions.h>
#include <functions.h>


void calculate_mark(char *student, int mark);
double calculate_mean();
void send_mean(double mean, char *write_fd);

int main(int argc, char *argv[])
{
    /*Install child's signal handler*/
    child_install_sh(getpid());

    printf("\t[X] PC[%d]: Calculating mark...\n", getpid());

    double mean = calculate_mean();

    sleep(2);
    my_write(LOG_PATH, APPEND_MOD, getpid(), 
    "Creación de archivos con nota necesaria para alcanzar la nota de corte, finalizada.\n");

    printf("\t[X] PC[%d]: marks have been calculated.\n", getpid());

    send_mean(mean, argv[1]);

    return EXIT_SUCCESS;
}

/*Send mean of student marks to manager by pipe*/
void send_mean(double mean, char *write_fd)
{
    char msg[SIZE];
    sprintf(msg, "%.2lf", mean);

    if(write(atoi(write_fd), msg, strlen(msg)) == ERROR){
        fprintf(stderr, "\t[X] PC[%d]: Error writting in the pipe\n", getpid());
        exit(EXIT_FAILURE);
    }
}

/*Calculate mean of student marks*/
double calculate_mean()
{
    FILE *file = open_file(STUDENTS_PATH, READ_MOD, getpid());
    int mark, i;
    char *student, line[MAX_SIZE];
    double mean = 0;

    for (i = 0; fgets(line, sizeof(line), file); i++)
    {
        student = strtok(line, SEPARATOR);
        strtok(NULL, SEPARATOR);
        mark = atoi(strtok(NULL, SEPARATOR));

        mean += mark;
        calculate_mark(student, mark);
    }

    fclose(file);

    return mean/i;
}

/*Calculate passing grade*/
void calculate_mark(char *student, int mark)
{   
    char path[SIZE] = "";
    sprintf(path, "directories/%s/passing-grade.txt", student);

    /*Calculate mark*/
    mark = 10 - mark;
    /*Add min mark*/
    if(mark < 4) mark = 4;

    char content[MAX_SIZE] = "";
    sprintf(content, "La nota que debes obtener en este nuevo examen para superar la prueba es %d.\n", mark);

    my_write(path, WRITE_MOD, getpid(), content);
}

