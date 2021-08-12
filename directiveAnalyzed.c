//
// Created by ronia on 06/08/2021.
//

#include "directiveAnalyzed.h"
#include "default.h"
#include "instructionAnalyzed.h"
#include "lableList.h"
#include "WordList.h"

Bool isDirectiveFirstPass(char *before, char *after, globalVariables *vars, Bool hasLabel, labelListPtr currentLabel,WordNodePtr currentWord) {
    /* long numOfBytes;*/
    int directiveNum , ValidLabelName , externLabel;
    Bool validDirectiveParam;
    Bool validAsciz,labelWithExtern;
    directiveNum = isValidDirectiveName(before); /*find if it's a valid directive and the num*/

    if (directiveNum != DIRECTIVE_ERROR && directiveNum != DIRECTIVE_EXTERN && directiveNum != DIRECTIVE_ENTRY) {
        DirectiveWordType directiveType = getDirectiveType(directiveNum);

        if (directiveNum == DIRECTIVE_BYTE || directiveNum == DIRECTIVE_HALF_WORD ||  directiveNum == DIRECTIVE_WORD) { /*dw,db,dh*/
            int validInput[LINE_LENGTH] = {0};
            validDirectiveParam = dataAnalysis(after, before, after, vars, validInput, directiveNum);
            if (validDirectiveParam == False) return False;
            /* numOfBytes = ((sizeof(validInput) / sizeof(int)) * directiveNum); */
            if (hasLabel == True)
                /*we have a label and a data - add to symbol table the value is the DC before insert the numbers to the list*/
            {
                ValidLabelName = labelNameCompare(&(vars->headLabelTable),currentLabel,vars); /*check if the label name wasn't shown in the table already*/
                if (ValidLabelName == VALID_LABEL) { /* a label isn't in the table*/
                    updateLabel(currentLabel,vars->DC,Data,NoEntryExtern);
                    //currentLabel->address = (vars->DC);
                    //currentLabel->codeOrData = Data;
                   // currentLabel->entryOrExtern = NoEntryExtern;
                    addLabelToList((&vars->headLabelTable), currentLabel);/*add the label to table*/
                    return True;
                } else { return False;  /*we found the label in the label table*/}
            }
            /*not a label only directive */
            addDirectiveByteToWordList(validInput, &(vars->headWordList), directiveNum, directiveType, vars->DC);
            return True;
        }
        if (directiveNum == DIRECTIVE_ASCIZ) {
            validAsciz = ascizAnalysis(after, vars);
            if (validAsciz == False) return False;
            /*a valid param to asciz directive a valid string starts and ends with " */
            if (hasLabel == True) { /*if the label flag is on - we have label*/
                ValidLabelName = labelNameCompare(&(vars->headLabelTable),currentLabel,vars); /*check if the label is already in the table, if it is - dont add to the table*/
                if (ValidLabelName == VALID_LABEL) { /* a label isn't in the table*/
                    updateLabel(currentLabel,vars->DC,Data,NoEntryExtern);
                    //currentLabel->address = (vars->DC);
                    //currentLabel->codeOrData = Data;
                    //currentLabel->entryOrExtern = NoEntryExtern;
                    addLabelToList(&(vars->headLabelTable), currentLabel);
                } else { return False; }  /*we found the label in the label table*/
            } else {
                /*no label just a directive - add to word table*/
                addDirectiveAsciz(after, &(vars->headWordList), directiveNum, directiveType, vars->DC);
            }
        }
    }
        /*not a db,dw,dh,asciz - check if an entry or extern or non=invalid directive*/
    else {
        labelAndEntryOrExtern(hasLabel, directiveNum,vars); /*if we have a label before entry or extern - it's not an error just ignore- don't insert label to label list*/
        if (directiveNum == DIRECTIVE_EXTERN) {  /*in the first pass if it's an external - enter tha label if it's correct one with dc of 0 and external type*/
            strip(after);
            externLabel = isLegalLabel(after, vars); /*check if its a valid label*/
            if (externLabel == LABEL_ERROR) { /*not a valid label - return an error*/
                return False;
            }
            /*else- a valid label check if already exists without external type or with */
            ValidLabelName = labelNameCompare(&(vars->headLabelTable), currentLabel, vars);
            labelWithExtern = isLabelExternal(&(vars->headLabelTable), currentLabel, vars);
            if (ValidLabelName == VALID_LABEL || labelWithExtern ==True) { /*label is not exists or if exists with external label and add to label table*/
                updateLabel(currentLabel,0,Data,Extern);
                //currentLabel->address = 0;
                //currentLabel->codeOrData = Data;
                //currentLabel->entryOrExtern = Extern;
                addLabelToList(&(vars->headLabelTable), currentLabel);
                return True;
            } else {
                return False;
            }
        } else {
            if (directiveNum == DIRECTIVE_ENTRY) {
                /*in the first pass if we see an entry label - don't do nothing just continue to the next row*/
                return True;
            } else { /*it's none of the directives return error*/
                return False;
            }
        }
    }
    return True;
}


Bool isDirectiveCommand(char command[LINE_LENGTH]) {
    return command[0] == '.' ? True : False;
}

int isValidDirectiveName(char *str)
{
    if(strcmp(str,".db")==0)
        return DIRECTIVE_BYTE; /*1*/
    if(strcmp(str,".dw")==0)
        return DIRECTIVE_WORD;  /*4*/
    if(strcmp(str,".dh")==0)
        return DIRECTIVE_HALF_WORD; /*2*/
    if(strcmp(str,".asciz")==0)
        return DIRECTIVE_ASCIZ; /*3*/
    if(strcmp(str,".extern")==0)
        return DIRECTIVE_EXTERN; /*5*/
    if(strcmp(str,".entry")==0)
        return DIRECTIVE_ENTRY; /*6*/
    else return DIRECTIVE_ERROR;/*-1*/
}

DirectiveWordType getDirectiveType(int directiveNum)
{
    if(directiveNum==1) return D_BYTE;
    if(directiveNum==2) return D_HALF;
    if(directiveNum==4) return D_WORD;
    if(directiveNum==3) return ASCIZ;

}

Bool dataAnalysis(char *str,char *before,char *after,globalVariables *vars,int validInput [LINE_LENGTH],int directive) {
    int number, i;
    int delimiter;
    long validBitRange;
    char line[LINE_LENGTH] = {0};
    int lastChar;

    strcpy(line, after);
    lastChar = strlen(line);

    if (line[lastChar] == ',') /* 3,4,5,7, situation*/
    {
        vars->type = ExtraneousComma;
        vars->errorFound = True;
        return False;
    }
    /*maybe add a check for non digit in the last char??*/

    for (i = 0; i < (LINE_LENGTH - 2) && line[i] != '\0'; i++) /* '\0' is the end of string char */
    {
        memset(line, 0, LINE_LENGTH);
        strcpy(line, after);

        memset(before, 0, LINE_LENGTH);
        memset(after, 0, LINE_LENGTH);

        delimiter = split(line, ",", before, after);
        strip(before);
        strip(after);

        if (delimiter == VALID_SPLIT) {
            strip(before);
            if (strlen(before) == 0 && i == 0) /*the first num starts without a comma before - ,3,4,...*/
            {
                vars->type = CommaBeforeFirstParam;
                /* printf("\n%s:Line %d:Illegal comma before the first param\n", vars->filename,  vars->currentLine); */
                vars->errorFound = True;
                return False;
            }
            if (strlen(before) == 0 && i != 0) /*+65,,7...*/
            {
                vars->type = CommaBetweenParams;
                // printf("\n%s:Line %d:Illegal comma between param\n", vars->filename,vars->currentLine); */
                vars->errorFound = True;
                return False;
            }
            number = isValidNumberDirective(before, vars); /*errors will update in the function*/
            if (number == INT_MIN) {
                return False; /*error - not a valid number*/
            }
            validBitRange = validNumByDirective(directive,
                                                number); /*check if the num is in the correct range according directive type*/
            if (validBitRange == VALID_BIT_RANGE) {
                validInput[i] = number;
                continue;
            } else {
                vars->type = ParamNotInBitRange;
                /* printf("\n%s:Line %d:number %d is not in bit range\n", vars->filename, vars->currentLine,number);*/
                vars->errorFound = True;
                return False;
            }

        } else {/*we couldn't find a comma*/
            if (strcmp(before, "") == 0) /*if we couldn't find a comma, by split function before gets line value,if empty- missing operands*/
            {
                vars->type = MissingOperand;
                /* printf("\n%s:Line %d:Missing Operand\n", vars->filename,vars->currentLine);*/
                vars->errorFound = True;
                return False;
            } else {/*not an empty string check if it's a valid operand*/
                number = isValidNumberDirective(before, vars); /*errors will update in the function*/
                if (number == INT_MIN) {
                    return False; /*error - not a valid number*/
                }
                validBitRange = validNumByDirective(directive, number); /*check if the num is in the correct range according directive type*/
                if (validBitRange == VALID_BIT_RANGE) {
                    validInput[i] = number;
                    continue;
                }
            }
        }
    }
    return True;
}

Bool ascizAnalysis(char *str,globalVariables *vars)
{
    int valid=isValidString(str);
    if(valid==VALID_STRING)
    {
        return True;
    }
    else{
        vars->type = StringNotValid;
        /*printf("\n%s:Line %d: %s not a valid string\n", vars->filename,vars->currentLine,str);*/
        vars->errorFound = True;
        return False;
    }
}



/*print a warning if we have a label before directive entry or extern*/
void labelAndEntryOrExtern(Bool hasLabel,int directiveNum,globalVariables *vars)
{
    if (directiveNum==DIRECTIVE_ENTRY && hasLabel==True) /*we have a label and a directive entry - error*/
    {
        vars->type=labelBeforeEntry;
        printf("\n%s:Line %d:Warning!Illegal Label before Entry\n", vars->filename,vars->currentLine);
    }
    if(directiveNum==DIRECTIVE_EXTERN && hasLabel==True)
    {
        vars->type=labelBeforeExtern;
         printf("\n%s:Line %d:Warning!Illegal Label before External\n", vars->filename,vars->currentLine);
    }
}
