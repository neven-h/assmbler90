//
// Created by Raz on 26/07/2021.
//

#include "firstPass.h"
#include "inputAnalyzed.h"
#include "instructionAnalysed.h"

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

    char fileName[FILE_NAME_LENGTH + AS_EXTENSION_LENGTH];
    strcpy(vars->filename, fileName);


    //symbolTable *lastLabel = NULL;
    //lastLabel=vars->headTable;
    Bool hasLabel;
    Bool isDirective;
    Bool directiveFirstPass;
    Bool isInstruction;
    Bool instructionFirstPass;
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

        lineAnalyzed = split(lineCpy, ":", before, after);
        if (lineAnalyzed == VALID_SPLIT)//*we found a ':' - Label*/
        {
            label = isLegalLabel(before, vars);
            /*to find if we have a label*/

            if (label == VALID_LABEL) {
                strip(before);
                strcpy(currentLabel->labelName, before);

                //lastLabel = lastLabel->next;
                hasLabel = True; /*label flag*/
            }
            strip(after);
            /*to find if after label it's instruction or directive*/

            lineAnalyzed = split(after, "\t\n", before, after);
            if (lineAnalyzed == VALID_SPLIT) {
                strip(before);
                strip(after);
                isDirective =isDirectiveCommand (before); /*if we find a '.' it's a directive*/
                if (isDirective == True) {
                    directiveFirstPass = isDirectiveFirstPass(before, after, vars, hasLabel, currentLabel, currentWord);
                    if(vars->errorFound==True) continue; /*if we found an error - continue to the next line **MAYBE PRINT BEFORE*/
                }
                /*if its not a directive - check an instruction*/
                instructionNum=instructionValidName (before);
                if(instructionNum!=INSTRUCTION_ERROR) {

                    instructionFirstPass = isInstructionFirstPass(before, after, vars, hasLabel, currentLabel,
                                                                  currentWord,instructionNum);
                }
                if(instructionNum==INSTRUCTION_ERROR && isDirective==False ) /*not a directive and not an instruction*/
                {
                    vars->type=notDirectiveOrInstruction;
                    // printf("\n%s:Line %d:Illegal we couldn't find an Instruction or Directive\n", vars->filename,
                    //       vars->currentLine);
                    vars->errorFound=True;
                    continue; /*get the next line*/
                }

            }
        }
        else { /*we couldn't find : so we dont have any label so to find directive */
            free(currentLabel);
            /*אם אין label אז צריך לבדוק שוב אם directive  או instruction לבחון את נושא הlabel וקוד חוזרני??*/


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
    int numOfBytes;
    int directiveNum;
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
        if (directiveNum == DIRECTIVE_ENTRY ) {
            if(hasLabel == VALID_LABEL) /* a label before entry is invalid*/
            {
                vars->type=labelBeforeEntry;
                // printf("\n%s:Line %d:Illegal Label before entry\n", vars->filename,
                //       vars->currentLine);
                vars->errorFound = True;
            }
            else{

            }

        } else {
                if(directiveNum == DIRECTIVE_EXTERN)
                {

                }

        }


    }

    if (directiveNum == DIRECTIVE_ERROR) return False;

    return True;
}


Bool isInstructionFirstPass(char *before, char *after, globalVariables *vars, Bool hasLabel, labelListPtr currentLabel,
                          WordNodePtr currentWord, int instructionNum)

{
    int ValidLabelName;
    int numOfOperands;
    Bool validRCommand;
   if(hasLabel==True)
   {
        ValidLabelName = labelNameCompare(vars->headLabelTable, currentLabel);
       if (ValidLabelName == VALID_LABEL) { /* a label isn't in the table*/
           currentLabel->value = (vars->IC);
           currentLabel->codeOrData = Code;
           currentLabel->entryOrExtern = NoEntryExtern;
           addLabelToList(vars->headLabelTable, currentLabel);/*add the label to table*/
       }
       else { if(ValidLabelName==LABEL_EXISTS)
           {
               vars->type=labelExistsInTable;
               // printf("\n%s:Line %d: Label already exists in table\n", vars->filename,
               //       vars->currentLine);
               vars->errorFound = True;
               return False;
           }
       }
   }
    if(hasLabel==False) /*we couldn't fond a label*/
    {
        free(currentLabel);
    }
    if (instructionNum<1 || instructionNum>27)
    {
        vars->type=IllegalInstruction;
        // printf("\n%s:Line %d: Instruction name is illegal \n", vars->filename,
        //       vars->currentLine);
        vars->errorFound = True;
        return False;
    }
    strip(before);
    InstructionWordType commandType = commandGroup(instructionNum);
    if (commandType==R_WORD)
    {
        currentWord->word.instruction.wordType=R_WORD;
        currentWord->word.instruction.rWord.opcode =0;
        currentWord->word.instruction.rWord.unused =0;
        numOfOperands=numberOfOperands(before,instructionNum);
        strip(after);
        if (numOfOperands == THREE_REGISTERS)
        {
            validRCommand=(after,before,after,instructionNum,numOfOperands,vars,currentWord);

        }
        if (numOfOperands == TWO_REGISTERS)

    }


}

