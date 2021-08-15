#include "main.h"



int main(int argc, char *argv[]) {
    int i;
    for(i = 1; i < argc; i++) {
        globalVariables *vars;
        char filename[FILE_NAME_LENGTH + AS_EXTENSION_LENGTH];
        vars = (globalVariables *) calloc(1, sizeof(globalVariables));
        sprintf(filename, "%s.as",argv[i]);

        strcpy(vars->filename, filename);

        vars->file = fopen(filename, "r");

        if(!vars->file) {
            printf("%s: Cannot open file!\n", filename);
            continue;
        }

        /* check that the file isn't empty */
        fseek(vars->file, 0, SEEK_END);
        if(ftell(vars->file) == 0)
            continue; //to the next file
        rewind(vars->file);

        resetVars(vars); //reset Vars values

        firstPass(vars);
        if(vars->errorFound == True) {
            printf("\n");
            freeVars(vars);
            continue;
        }

        printf("Completed First Pass!\n");
        rewind(vars->file);

       secondPass(vars);
        if(vars->errorFound == True) {
            printf("\n");
            freeVars(vars);
            continue;
        }

        printf("Completed Second Pass!\n");
       // createOutput(vars);
        freeVars(vars);
        printf("\n");
    }
    return 0;
}

void resetVars(globalVariables *vars) {
    vars->IC = 100;
    vars->DC = 0;
    vars->DCF = 0;
    vars->ICF = 0;
    vars->errorFound = False;
    vars->currentLine = 1;
    vars->headLabelTable= NULL;
    vars->headWordList=NULL;
    vars->headExternList=NULL;
    vars->headEntryList=NULL;
}

void freeVars(globalVariables *vars) {
    free(vars);
}


