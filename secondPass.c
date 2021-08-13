//
// Created by ronia on 08/08/2021.
//

#include "secondPass.h"


void secondPass(globalVariables *vars) {

    int lineAnalyzed,instructionNum;
    Bool hasLabel,EntryLabel;
    WordType word;
    Bool directiveSecondPass;
    Bool instructionSecondPass;
    int ICcounter=100;


    char line[LINE_LENGTH] = {0};
    char before[LINE_LENGTH] = {0};
    char after[LINE_LENGTH] = {0};
    char lineCpyAfterLabel[LINE_LENGTH] = {0};

    char fileName[FILE_NAME_LENGTH + AS_EXTENSION_LENGTH];
    strcpy(vars->filename, fileName);

    while (!feof(vars->file)) {


        memset(line, 0, LINE_LENGTH);
        memset(before, 0, LINE_LENGTH);
        memset(after, 0, LINE_LENGTH);
        memset(lineCpyAfterLabel, 0, LINE_LENGTH);

        fgets(line, LINE_LENGTH, stdin);

        strip(line); /*strip white chars*/
        lineAnalyzed = isEmptyOrCommandLine(line);
        if (lineAnalyzed == 1) continue; /*if the line is an empty line or command line - the assembler ignores*/

        /*create a new label node*/
        labelListPtr currentLabel = (labelListPtr) calloc(1, sizeof(labelListPtr));
        if (!currentLabel) {
            exit(0);
        }

        /*check if we have label - if we do - skip this label*/
        /*add bool function to check if we have a label*/
        hasLabel = foundLabel(line, before, after, vars, currentLabel);

        if (hasLabel == True) { /*we found a label*/
            strcpy(lineCpyAfterLabel, after);
        } else { /*we couldn't fina d label, by split fun before=line*/
            strcpy(lineCpyAfterLabel, line);
        }

        strip(lineCpyAfterLabel);
        word = directiveOrInstruction(lineCpyAfterLabel, before, after, vars); /*check if Directive or Instruction or none*/
        if (word == Directive) {
            directiveSecondPass = isDirectiveSecondPass(before, after, vars, hasLabel, currentLabel);
            if (directiveSecondPass == False)
                continue; /* not an ENTRY directive - continue to the next line*/
            if(directiveSecondPass ==True) /*it's an Entry Directive*/
            {
                EntryLabel=isLabelEntry(&(vars->headLabelTable),after,vars); /*check if the given entry label exists and add to the label list the attribute to this label as -entry*/
                if(EntryLabel==False) /*not a valid label*/
                    printErrors(vars);
            }
        }
        else{ /*if it is not a directive it must be an instruction*/
            instructionNum = instructionValidName(before); /*get the instruction number*/
            InstructionWordType commandType = commandGroup(instructionNum);
            if((instructionNum>=BNE && instructionNum<=BGT)) /*an I - Branch instruction - calculate the sub between*/
            {
                instructionSecondPass=secondPassI(after,vars,ICcounter,commandType);
            }
            else {
                if ((instructionNum >= JMP && instructionNum <= CALL)) /*an J - Branch instruction*/
                {
                    instructionSecondPass = secondPassJ(after, vars, ICcounter, commandType);
                }
            }
            if(instructionSecondPass==False)
                printErrors(vars);
            ICcounter+=4;
        }

    }
}



