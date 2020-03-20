#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <definitions.h>
#include <functions.h>

void create_root_dir();
void create_directory(char *student);


int main()
{
    /*Install child's signal handler*/
    child_install_sh(getpid());

    FILE *file = open_file(STUDENTS_PATH, READ_MOD, getpid());
    char line[MAX_SIZE];

    my_write(LOG_PATH, APPEND_MOD, getpid(), "Creación de directorios finalizada.\n");
    create_root_dir();

    printf("\t[X] PA[%d]: Creating directories...\n", getpid());

    /*Load student's directory from students.txt*/
    while(fgets(line, sizeof(line), file)){

        /*Create directory (First column of file)*/
        create_directory(strtok(line, SEPARATOR));
    }                                                              
    
    sleep(2);
    printf("\t[X] PA[%d]: Directories have been created.\n", getpid());
    
    fclose(file);

    return EXIT_SUCCESS;
}

/*Create 'directories'(root directory for the students directories)*/
void create_root_dir(){
    if (opendir(DIRECTORIES_PATH) == ERRNO)
    { 
        if(mkdir("directories", MKDIR_MOD) == ERROR)
        {
            fprintf(stderr, "\t[X] PA[%d]: Error create root directory.\n",getpid());
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        fprintf(stderr, "\t[X] PA[%d]: '%s' directory already exists.\n", getpid(), DIRECTORIES_PATH);
        exit(EXIT_FAILURE);
        
    }
    
}

/*Create student directory*/
void create_directory(char *student)
{
    char path[SIZE] = "directories/";
    strcat(path, student);
    if (mkdir(path, MKDIR_MOD) == ERROR)
    {
        fprintf(stderr, "\t[X] PA[%d]: Error create directory.\n",getpid());
        exit(EXIT_FAILURE);
    }
}

