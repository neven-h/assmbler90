//
// Created by Raz on 26/07/2021.
//

#include "firstPass.h"
#include "inputAnalyzed.h"
#include "instructionAnalyzed.h"
#include "directiveAnalyzed.h"

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

    char fileName[FILE_NAME_LENGTH + AS_EXTENSION_LENGTH];
    strcpy(vars->filename, fileName);


    //symbolTable *lastLabel = NULL;
    //lastLabel=vars->headTable;
    Bool hasLabel;
    Bool isDirective;
    Bool directiveFirstPass;
    Bool isInstruction;
    Bool instructionFirstPass;
    WordType word;

    int i, lineAnalyzed, label, directive;
    int numOfBytes;
    int instructionNum;

    while (!feof(vars->file)) {


        memset(line, 0, LINE_LENGTH);
        memset(lineCpy, 0, LINE_LENGTH);
        memset(before, 0, LINE_LENGTH);
        memset(after, 0, LINE_LENGTH);

        fgets(line, LINE_LENGTH, stdin);

        /*checks if the row is in the length boundaries*/
        for (i = 0; i <= LAST_CHAR_IN_LINE; i++) {
            if (line[i] == '\0') {
                strncpy(lineCpy, line, i + 1);
                vars->currentLine++;
                break;
            }
        }
        if (((i - 1) == LAST_CHAR_IN_LINE) && line[i - 1] != '\0') {
            vars->type = LineTooLong;
            vars->errorFound = True;
        }

        strip(lineCpy); //*strip white chars*/
        lineAnalyzed = isEmptyOrCommandLine(lineCpy);
        if (lineAnalyzed == 1) continue; //*if the line is an empty line or command line - the assembler ignores*/

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
        hasLabel = foundLabel(lineCpy, before, after, vars, currentLabel);

        if (hasLabel == True) { /*we found a label*/
            strcpy(lineCpyAfterLabel, after);
        } else { /*we couldn't fina d label, by split fun before=linecpy*/
            strcpy(lineCpyAfterLabel, lineCpy);
        }

        strip(lineCpyAfterLabel);
        word = directiveOrInstruction(lineCpyAfterLabel, before, after,
                                      vars); /*check if Directive or Instruction or none*/
        if (word == Directive) {


            directiveFirstPass = isDirectiveFirstPass(before, after, vars, hasLabel, currentLabel, currentWord);
            if (directiveFirstPass == False) continue; /*if we found an error - continue to the next line*/
        } else {
            if (word == Instruction) {

                directiveFirstPass = isDirectiveFirstPass(before, after, vars, hasLabel, currentLabel, currentWord);
                if (vars->errorFound == True)
                    continue; /*if we found an error - continue to the next line **MAYBE PRINT BEFORE*/
            } else { /*not a directive and not an instruction than - None - error*/
                continue; /*get the next line*/
            }

            /*if its not a directive - check an instruction*/
            instructionNum = instructionValidName(before);
            if (instructionNum != INSTRUCTION_ERROR) {

                instructionFirstPass = isInstructionFirstPass(before, after, vars, hasLabel, currentLabel,
                                                              currentWord, instructionNum);
            }
        }
    }
}

void getToNextLine(FILE *f) {
    int c;
    while ((c = fgetc(f)) != EOF) {
        if (c == '\n')
            return;
    }
}

Bool isDirectiveFirstPass(char *before, char *after, globalVariables *vars, Bool hasLabel, labelListPtr currentLabel,
                          WordNodePtr currentWord) {
    long numOfBytes;
    int directiveNum;
    Bool labelBeforeEntryOrExtern;
    directiveNum = isValidDirectiveName(before); /*find if it's a valid directive and the num*/

    if (directiveNum != DIRECTIVE_ERROR && directiveNum != DIRECTIVE_EXTERN &&
        directiveNum != DIRECTIVE_ENTRY) {
        DirectiveWordType directiveType = getDirectiveType(directiveNum);

        if (directiveNum == DIRECTIVE_BYTE || directiveNum == DIRECTIVE_HALF_WORD ||
            directiveNum == DIRECTIVE_WORD) { /*dw,db,dh*/

            int validInput[LINE_LENGTH] = {0};
            dataAnalysis(after, before, after, vars, validInput, directiveNum);
            numOfBytes = ((sizeof(validInput) / sizeof(int)) * directiveNum);
            if (hasLabel == True)

                /*we have a label and a data - add to symbol table the value is the DC before insert the numbers to the list*/
            {
                int ValidLabelName = labelNameCompare(vars->headLabelTable, currentLabel);
                if (ValidLabelName == VALID_LABEL) { /* a label isn't in the table*/
                    currentLabel->value = (vars->DC);
                    currentLabel->codeOrData = Data;
                    currentLabel->entryOrExtern = NoEntryExtern;
                    addLabelToList(vars->headLabelTable, currentLabel);/*add the label to table*/
                }

            }

            /*not a label only directive */
            addDirectiveByteToWordList(validInput, vars->headWordList, directiveNum, directiveType, vars->DC);
            //vars->DC += (numOfBytes);
            //currentLabel->codeOrData = Data;
            //currentLabel->entryOrExtern = NoEntryExtern;
        }

        if (directiveNum == DIRECTIVE_ASCIZ) {

            ascizAnalysis(after, vars);
            if (hasLabel == VALID_LABEL) {
                int ValidLabelName = labelNameCompare(vars->headLabelTable, currentLabel);
                currentLabel->value = (vars->DC);
                currentLabel->codeOrData = Data;
                currentLabel->entryOrExtern = NoEntryExtern;
                addLabelToList(vars->headLabelTable, currentLabel);

            } else {
                /*add to word table*/
                addDirectiveAsciz(after, vars->headWordList, directiveNum, directiveType, vars->DC);
                //vars->DC += (numOfBytes + 1);
            }
        }
    }
        /*not a db,dw,dh,asciz - check if an entry or extern or non=invalid directive*/
    else {
        labelBeforeEntryOrExtern = labelAndEntryOrExtern(hasLabel, directiveNum, vars);
        if (labelBeforeEntryOrExtern == False) {
            return False; /*it's invalid to have label before entry or extern*/
        } else {
            if (directiveNum == DIRECTIVE_ENTRY) {

            }


        }


    }

    if (directiveNum == DIRECTIVE_ERROR) return False;
    return True;

}


Bool isInstructionFirstPass(char *before, char *after, globalVariables *vars, Bool hasLabel, labelListPtr currentLabel,
                            WordNodePtr currentWord, int instructionNum) {
    int ValidLabelName;
    int numOfOperands;
    Bool validRCommand, validICommand;

    currentWord->word.wordType = Instruction;
    if (hasLabel == True) {
        ValidLabelName = labelNameCompare(vars->headLabelTable, currentLabel);
        if (ValidLabelName == VALID_LABEL) { /* a label isn't in the table*/
            currentLabel->value = (vars->IC);
            currentLabel->codeOrData = Code;
            currentLabel->entryOrExtern = NoEntryExtern;
            addLabelToList(vars->headLabelTable, currentLabel);/*add the label to table*/
        } else {
            if (ValidLabelName == LABEL_EXISTS) {
                vars->type = labelExistsInTable;
                // printf("\n%s:Line %d: Label already exists in table\n", vars->filename,
                //       vars->currentLine);
                vars->errorFound = True;
                return False;
            }
        }
    }
    if (hasLabel == False) /*we couldn't fond a label*/
    {
        free(currentLabel);
    }
    if (instructionNum < 1 || instructionNum > 27) {
        vars->type = IllegalInstruction;
        // printf("\n%s:Line %d: Instruction name is illegal \n", vars->filename,
        //       vars->currentLine);
        vars->errorFound = True;
        return False;
    }
    strip(before);
    InstructionWordType commandType = commandGroup(instructionNum);
    if (commandType == R_WORD) {
        currentWord->word.instruction.wordType = R_WORD;
        numOfOperands = numberOfOperands(commandType, instructionNum);
        strip(after);
        validRCommand = R_commandAnalyzed(after, before, after, instructionNum, numOfOperands, vars, currentWord);
        if (validRCommand == True) {
            /*need to add the word node to the list*/
            vars->IC = (vars->IC + 4);
            /*add coding to binary?!*/
            return True;
        } else {
            return False; /*not valid R Command*/
        }
    }
    if (commandType == I_WORD) {
        currentWord->word.instruction.wordType = I_WORD;
        int type = numberOfOperands(commandType, instructionNum);
        strip(after);
        validICommand = I_commandAnalyzed(after, before, after, instructionNum, type, vars, currentWord);
        if (validICommand == True) {
            /*need to add the word node to the list*/
            vars->IC = (vars->IC + 4);
            /*add coding to binary?!*/
            return True;
        } else {
            return False; /*not valid I Command*/
        }
    }
    if (commandType == J_WORD) {
        currentWord->word.instruction.wordType = J_WORD;
        int type = numberOfOperands(commandType, instructionNum);
        strip(after);
        validICommand = J_commandAnalyzed(after, before, after, instructionNum, type, vars, currentWord);
        if (validICommand == True) {
            /*need to add the word node to the list*/
            vars->IC = (vars->IC + 4);
            /*add coding to binary?!*/
            return True;
        } else {
            return False; /*not valid I Command*/
        }

    }


}

