//
// Created by ronia on 06/08/2021.
//

#include "directiveAnalyzed.h"
#include "default.h"
#include "instructionAnalyzed.h"




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

void dataAnalysis(char *str,char *before,char *after,globalVariables *vars,int validInput [LINE_LENGTH],int directive)
{
    int number;
    int counter=0;
    int delimiter;
    long validBitRange;
    while(counter<LINE_LENGTH)
    {
        //to check the stop condition - i think i will have memory

        delimiter= split(str,",",before,after);
        if (delimiter==VALID_SPLIT)
        {
            strip(before);
            if(strlen(before)==0 && counter==0) /*the first num is without a ,*/
            {
                vars->type=CommaBeforeFirstParam;
                // printf("\n%s:Line %d:Illegal comma before the first param\n", vars->filename,
                //      vars->currentLine);
                vars->errorFound = True;
                continue;
            }
            if(strlen(before)==0 && counter!=0) /*+65,,7...*/
            {
                vars->type=CommaBetweenParams;
                // printf("\n%s:Line %d:Illegal comma between param\n", vars->filename,
                //      vars->currentLine);
                vars->errorFound = True;
                continue;
            }
            number=isValidNumberDirective(before,vars);
            validBitRange=validNumByDirective(directive,number);
            if(validBitRange==VALID_BIT_RANGE){
                validInput[counter]=number;
                counter++;

            }
            else{
                vars->type=ParamNotInBitRange;
                // printf("\n%s:Line %d:number %d is not in bit range\n", vars->filename,
                //      vars->currentLine,number);
                vars->errorFound = True;
                continue;
            }

        }
    }
}

void ascizAnalysis(char *str,globalVariables *vars)
{
    int valid=isValidString(str);
    if(valid==VALID_STRING)
    {
        /*WHAT TO DO HERE?!!?*/
    }
    else{
        printf("\n%s:Line %d: %s not a valid string\n", vars->filename,
               vars->currentLine,str);
        vars->errorFound = True;
    }
}
/*print a warning if we have a label before directive entry or extern*/
void labelAndEntryOrExtern(Bool hasLabel,int directiveNum,globalVariables *vars)
{
    if (directiveNum==ENTRY && hasLabel==True) /*we have a label and a directive entry - error*/
    {
        vars->type=labelBeforeEntry;
        /* printf("\n%s:Line %d:Warning!Illegal Label before Entry\n", vars->filename,vars->currentLine);*/
    }
    if(directiveNum==EXTERNAL && hasLabel==True)
    {
        vars->type=labelBeforeExtern;
         /* printf("\n%s:Line %d:Warning!Illegal Label before External\n", vars->filename,vars->currentLine);*/
    }
}
