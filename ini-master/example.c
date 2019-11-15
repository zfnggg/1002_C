#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ini.h"

int main(){
    /* A showcase of how to use the ini lib */
    char answer[100];
    char status[25];

    // Loading the file into config
    ini_t *config = ini_load("ICT1002_Group Project Assignment_AY19_T1_Sample.ini");

    // Writing a new learnt answer to the .ini file given an intent and the entity
    // If intent and entiry already exists, the answer in the file will be updated
    // How to use: ini_write(config, intent, entity, answer)
    ini_write(config, "where", "SIT", "malloc");

    // Searching for the answer to a certain entity given an intent
    // ini_get(config, intent, entity, string) -> returns a status and puts the answer in string
    strcpy(status, ini_get(config, "chocolate", "new", answer));
    if (strcmp(status, "KB_OK") == 0){
        printf("%s\n", answer);
    }
    else {
        printf("%s", status);
    }

    return 0;
}