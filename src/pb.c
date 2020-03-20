#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <functions.h>
#include <definitions.h>


void copy_models();
void copy_single_model(char *student, char model);


int main()
{
    /*Install child's signal handler*/
    child_install_sh(getpid());

    printf("\t[X] PB[%d]: Copiying exam models...\n", getpid());
    
    copy_models();

    sleep(2);
    my_write(LOG_PATH, APPEND_MOD, getpid(), "Copia de modelos de examen, finalizada.\n");
    
    printf("\t[X] PB[%d]: Exam models have been copied.\n", getpid());
    
    return EXIT_SUCCESS;
}

/*Copy the exam models*/
void copy_models(){
    FILE *file = open_file(STUDENTS_PATH, READ_MOD, getpid());

    char line[MAX_SIZE];
    char *student;
    char model;

    while (fgets(line, sizeof(line), file))
    {
        student = strtok(line, SEPARATOR);
        model = *strtok(NULL, SEPARATOR);
        copy_single_model(student, model);
    }

    fclose(file);

}

/*Copy a single exam model*/
void copy_single_model(char *student, char model){
    char command[MAX_SIZE] = "";
    sprintf(command, "cp files/MODELO%c.pdf directories/%s", model, student);
    
    /*Copy student's exam model*/
    system(command);

}

