#include <stdio.h>
#include <stdlib.h>
#include <definitions.h>
#include <string.h>
#include <unistd.h>
#include <functions.h>

int main(){

    printf("\t[X] PD[%d]: Removing datas...\n", getpid());

    char command[SIZE] = "rm -r ";
    strcat(command, DIRECTORIES_PATH);
    
    if(system(command) == ERROR){
        fprintf(stderr, "\t[X] PD[%d]: Error executing remove command.\n", getpid());
        exit(EXIT_FAILURE);
    }

    printf("\t[X] PD[%d]: Datas have been removed.\n", getpid());
    
    my_write(LOG_PATH, WRITE_MOD, getpid(), 
    "Ha surgido una interrupción por parte del usuario, los datos han sido eliminados.");
    
    return EXIT_SUCCESS;

}