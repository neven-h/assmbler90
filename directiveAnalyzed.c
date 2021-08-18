//
// Created by ronia on 06/08/2021.
//

#include "directiveAnalyzed.h"
#include "instructionAnalyzed.h"

Bool isDirectiveFirstPass(char *before, char *after,char *label ,globalVariables *vars, Bool hasLabel, labelListPtr currentLabel,WordNodePtr currentWord) {

    int directiveNum , ValidLabelName;
    Bool validDirectiveParam;
    Bool validAsciz,labelBeforeDirective;
    directiveNum = isValidDirectiveName(before); /*find if it's a valid directive and the num*/

    /* first pass step 6 */
    if (directiveNum != DIRECTIVE_ERROR ) {
        DirectiveWordType directiveType = getDirectiveType(directiveNum); /*find the directive word type*/

        if (directiveNum == DIRECTIVE_BYTE || directiveNum == DIRECTIVE_HALF_WORD || directiveNum == DIRECTIVE_WORD) { /*dw,db,dh*/
            long validInput[LINE_LENGTH] = {0};
            validDirectiveParam = dataAnalysis(after, before, after, vars, validInput,directiveNum);/*analyzed the operands of directive row*/
            if (validDirectiveParam == False) return False;
            if (hasLabel == True)
                /*we have a label and a data - add to symbol table the value is the DC before insert the numbers to the list*/
            {
                labelBeforeDirective = labelBeforeDirectiveCommand(label, vars, currentLabel);
                if (labelBeforeDirective == False) return False; /*if False - return false and get the next row, else continue*/

            }
            /*not a label only directive */
            addDirectiveByteToWordList(validInput, &(vars->headWordList), directiveNum, directiveType, vars);
            return True;
        }
        if (directiveNum == DIRECTIVE_ASCIZ) {
            validAsciz = ascizAnalysis(after, vars);
            if (validAsciz == False) return False;
            /*a valid param to asciz directive a valid string starts and ends with " */

            if (hasLabel == True) { /*if the label flag is on - we have label*/
                labelBeforeDirective = labelBeforeDirectiveCommand(label, vars, currentLabel);
                if (labelBeforeDirective == False) return False; /*if False - return false and get the next row, else continue*/
            }
            /*no label just a directive - add to word table*/
            addDirectiveAsciz(after, &(vars->headWordList), directiveType, vars);
            return True;
        }
        /*not a db,dw,dh,asciz - check if an entry or extern or non=invalid directive*/
        labelAndEntryOrExtern(hasLabel, directiveNum,vars); /*if we have a label before entry or extern - it's not an error just ignore- don't insert label to label list*/
        if (directiveNum == DIRECTIVE_EXTERN) {
            Bool externFirstPass = externDirectiveFirstPass(after, vars, currentLabel);
            if (externFirstPass == False)return False;
            else { return True; }
        }
        /*directiveNum == DIRECTIVE_ENTRY -in the first pass if we see an entry label - don't do nothing just continue to the next row**/
        return True;
    }
    return False; /*it's not a valid directive*/
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

Bool dataAnalysis(char *str,char *before,char *after,globalVariables *vars,long validInput [LINE_LENGTH],int directive) {
    int number, i;
    int counter=0;
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
            validBitRange = validNumByDirective(directive, number); /*check if the num is in the correct range according directive type*/
            if (validBitRange == VALID_BIT_RANGE) {
                validInput[i] = number;
                counter++;
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
                if (number == LONG_MIN) {
                    return False; /*error - not a valid number*/
                }
                validBitRange = validNumByDirective(directive, number); /*check if the num is in the correct range according directive type*/
                if (validBitRange == VALID_BIT_RANGE) {
                    validInput[i] = number;
                    counter++;
                    break; /*couldn't find a comma and we updates the last number in array*/
                }
            }
        }
    }
    /*block the int array*/
    validInput[counter]=LONG_MAX;
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


Bool isDirectiveSecondPass(char *before,char *after ,globalVariables *vars, Bool hasLabel, labelListPtr currentLabel) {
    int directiveNum;
    directiveNum = isValidDirectiveName(before); /*find if it's a valid directive and the num*/
    if (directiveNum != DIRECTIVE_ENTRY)
    {
        return False; /*if it's not an entry continue to the next line*/
    }
    /*an entry label */
    return True;

}
/*This function handles cases where there is a label before db,dw,dh,asciiz directives. check if the label is already in the label list and if not update the label list*/
Bool labelBeforeDirectiveCommand(char *labelName, globalVariables *vars, labelListPtr currentLabel)
{
    int ValidLabelName;
    ValidLabelName = labelNameCompare(&(vars->headLabelTable),labelName,vars); /*check if the label name wasn't shown in the table already*/
    if (ValidLabelName == VALID_LABEL) { /* a label isn't in the table*/
        updateLabel(currentLabel,vars->DC,Data,NoEntryExtern); /*update the current label node*/
        addLabelToList((&vars->headLabelTable), currentLabel);/*adds the label to table */
        return True;
    }

    else{ return False;  /*we found the label in the label table*/}
}

Bool externDirectiveFirstPass(char *after ,globalVariables *vars,labelListPtr currentLabel)
{
    int ValidLabelName;
    Bool labelWithExtern,externLabel;

    strip(after);
    externLabel = isLegalLabel(after, vars); /*check if  the operand label is valid label by syntax*/
    if (externLabel == LABEL_ERROR) { /*not a valid label - return an error*/
        return False;
    }
    /*else- a valid label check if already exists without external type or with */
    ValidLabelName = labelNameCompare(&(vars->headLabelTable), after, vars);
    labelWithExtern = isLabelExternal(&(vars->headLabelTable), after, vars);
    if (ValidLabelName == VALID_LABEL || labelWithExtern ==True) {
        /*label is not exists or if exists with external label and add to label table*/
        strcpy(currentLabel->labelName,after); /*update the label we want to add name*/
        updateLabel(currentLabel,0,NoCodeOrData,Extern);
        addLabelToList(&(vars->headLabelTable), currentLabel);/*add to label list*/

        return True;
    } else {
        return False;
    }
}