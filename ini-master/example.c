#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ini.h"

int main(){
    /* A showcase of how to use the ini lib */
    char entity[100];

    // Loading the file into config
    ini_t *config = ini_load("ICT1002_Group Project Assignment_AY19_T1_Sample.ini");

    // Writing a new learnt entity to the .ini file given an intent
    // How to use: ini_write(config, intent, entity, file_to_output)
    ini_write(config, "where", "new=ICT1005", "../output.txt");

    // Searching for the answer to a certain entity given an intent
    // ini_get(config, intent, entity) -> returns a pointer to a string
    if (ini_get(config, "what", "SIT")){
        printf("%s\n", ini_get(config, "what", "SIT"));
    }
    

    return 0;
}