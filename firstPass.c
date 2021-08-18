//
// Created by Raz on 26/07/2021.
//

#include "firstPass.h"


void readLine(char *[]);

void getToNextLine(FILE *f);

void readLine(char *line[]) {
    fgets(*line, LINE_LENGTH, stdin);

    size_t len;
    len = strlen(*line);

    /* check if the length of the line length is valid (80 chars exclude \n\0), if invalid go to the next line */
    if (len == LINE_LENGTH - 1 && (*line)[len - 1] != '\0' && (*line)[len - 1] != '\n') {
        getToNextLine(stdin);
    }

}

void firstPass(globalVariables *vars) {

    char line[LINE_LENGTH] = {0};
    char lineCpy[LINE_LENGTH] = {0};
    char before[LINE_LENGTH] = {0};
    char after[LINE_LENGTH] = {0};
    char lineCpyAfterLabel[LINE_LENGTH] = {0};
    char label[LABEL_LENGTH] = {0};

    char fileName[FILE_NAME_LENGTH + AS_EXTENSION_LENGTH];
    strcpy(vars->filename, fileName);


    Bool hasLabel,validLineLength;
    Bool directiveFirstPass;
    Bool instructionFirstPass;
    WordType word;


    int  lineAnalyzed;
    int instructionNum;


    while (!feof(vars->file)) {


        resetStrings(line,lineCpy,before,after,lineCpyAfterLabel,label);

        fgets(line, LINE_LENGTH, vars->file); /*vars->file*/

        validLineLength=getLine(line,lineCpy,vars);
        if (validLineLength==False)continue; /*get the next line*/


        strip(lineCpy); /*strip white chars*/
        lineAnalyzed = isEmptyOrCommandLine(lineCpy);
        if (lineAnalyzed == 1) continue; /*if the line is an empty line or command line - the assembler ignores*/
        vars->currentLine++;
        /*analyze if its a directive or instruction*/

        /*create a new word node*/
        WordNodePtr currentWord = (WordNodePtr) calloc(1, sizeof(WordNode));
        if (!currentWord) {
            exit(0);
        }

        /*create a new label node*/
        labelListPtr currentLabel = (labelListPtr) calloc(1, sizeof(labelListPtr));
        if (!currentLabel) {
            exit(0);
        }

        /*add bool function to check if we have a label*/
        hasLabel = foundLabel(lineCpy, before, after, vars, currentLabel); /*look for a label and the checks if it's a valid label*/

        if (hasLabel == True) { /*we found a label*/
            strcpy(label, before);
            strcpy(lineCpyAfterLabel, after);
        } else { /*we couldn't fina d label, by split fun before=linecpy*/
            strcpy(lineCpyAfterLabel, lineCpy);
        }


        strip(lineCpyAfterLabel);
        word = directiveOrInstruction(lineCpyAfterLabel, before, after, vars); /*check if Directive or Instruction or none*/
        if (word == Directive) {
            directiveFirstPass = isDirectiveFirstPass(before, after,label ,vars, hasLabel, currentLabel, currentWord);
            if (directiveFirstPass == False) {
                printErrors(vars);
            }

        } else {
            if (word == Instruction) {
                instructionNum = instructionValidName(before); /*get the instruction number*/
                instructionFirstPass = isInstructionFirstPass(before, after,label, vars, hasLabel, currentLabel, currentWord, instructionNum);
                if (instructionFirstPass == False )
                    printErrors(vars); /* we found an error -  print and continue to the next line not a valid instruction line*/
            } else { /*not a directive and not an instruction than - None - error*/
                vars->type = notDirectiveOrInstruction;
                /* printf("\n%s:Line %d:Illegal we couldn't find an Instruction or Directive\n", vars->filename, vars->currentLine);*/
                vars->errorFound = True;
                printErrors(vars);
            }
        }

    }
    /*we finished to read the file*/
    if(vars->errorFound == False) /*we found errors - don't continue to second Pass */
    {
        /*we haven't found any errors */
        vars->ICF=vars->IC;
        vars->DCF=vars->DC;

        updateLabelTableICF(&(vars->headLabelTable),vars); /*update the value of data labels with final IC*/
        addDirectiveICF(&(vars->headWordList),vars); /*add the final IC value to the directive values in Word list*/
    }

}




void getToNextLine(FILE *f) {
    int c;
    while ((c = fgetc(f)) != EOF) {
        if (c == '\n')
            return;
    }
}

Bool getLine(char *line,char *lineCpy,globalVariables *vars)
{
    /*checks if the row is in the length boundaries*/
   int i;
    for (i = 0; i <= LAST_CHAR_IN_LINE; i++) {
        if (line[i] == '\0') {
            strncpy(lineCpy, line, i + 1);
            break;
        }
    }
    /*Check if the line we got is to long - more than 80 chars*/
    if (((i - 1) == LAST_CHAR_IN_LINE) && line[i - 1] != '\0') {
        vars->type = LineTooLong;
        vars->errorFound = True;
        printErrors(vars);
        getToNextLine(vars->file);
        return False;
    }
    return True;
}
