//
// Created by ronia on 27/07/2021.
//

#include "inputAnalyzed.h"
#include "default.h"

/*strip the given string from the white chars in the beginning and the end*/
void strip(char *str) {
    char newString[LINE_LENGTH] = {0};
    int left = 0;
    int right = strlen(str) - 1;

    while (isspace(str[left]) && left < right) {
        left++;
    }
    while (isspace(str[right]) && left < right) {
        right--;
    }
    strncpy(newString, str + left, right - left + 1);
    memset(str, 0, LINE_LENGTH);
    strcpy(str, newString);
}

int split(char *str, char *delimiter, char *before, char *after) {
    int i, j;
    memset(after, 0, LINE_LENGTH);
    memset(before, 0, LINE_LENGTH);
    for (i = 0; i < strlen(str); i++) {
        for (j = 0; j < strlen(delimiter); j++) {
            if (delimiter[j] == str[i]) {
                strncpy(before, str, i);
                strcpy(after, str + i + 1);
                return 1;
            }
        }
    }
    strcpy(before, str);
    return -1;
}

int isLegalLabel(char *str, globalVariables *vars) {
    if (!isspace(strlen(str) - 1))//*a legal label*
    {
        strip(str);
        int length = strlen(str);
        if (length <= LABEL_LENGTH)//לבדוק label של 31 תווים אם נכנס באורך מבחינת תו אחרון
        {
            int first = (int) str[0];
            if (isalpha(first)) //*a legal label starts with an alphabet */
            {
                int i, isAlphaNum;
                for (i = 0; i < strlen(str); i++) {
                    isAlphaNum = isalnum((int) str[i]);
                    if (isAlphaNum == 0)//*neither an alphabet nor a digit.*/
                    {
                        vars->type=IllegalCharInLabel;
                       // printf("\n%s:Line %d:Illegal Label char %c neither an alphabet nor a digit\n", vars->filename,
                       //     vars->currentLine, str[i]);
                        vars->errorFound = True;
                        return LABEL_ERROR;
                    }
                }

            } else {
                vars->type=firstCharInLabelNotAlphabet;
               // printf("\n%s:Line %d:Illegal Label first char %c not an alphabet\n", vars->filename,
                //       vars->currentLine, str[0]);
                vars->errorFound = True;
                return LABEL_ERROR;
            } //*not a legal label first char*/
        }
    } else {
        vars->type=TooLongLabel;
      //  printf("\n%s:Line %d:Illegal Label more than 31 characters\n", vars->filename,
      //         vars->currentLine);
        vars->errorFound = True;
        return LABEL_ERROR;/*more than 31 char*/
    }

    return VALID_LABEL;
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

int isEmptyOrCommandLine(char *str) {
    if (str[0] == '\0' || str[0] == ';')
        return 1;
    else return -1;
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
            number=isValidNumber(before,vars);
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

int isValidNumber(char *str,globalVariables *vars)
{
    int sign=1;
    int i;
    int number;
    char num[LINE_LENGTH]={0};
    if((isdigit((int)(str[0]))==0)) /*doesnt start with a digit*/
    {
        if(str[0]=='+')sign=1;/*positive num*/
        if(str[0]=='-')sign=-1;/*negative num*/
        strcpy(num,str+1);
    }
    else {strcpy(num,str);} /*start with a digit*/

    for(i=0;i< strlen(num);i++)
    {
        int Digit=(int)(num[i]);
        if(isdigit(Digit)==0)
        {
            printf("\n%s:Line %d: %s not an integer\n", vars->filename,
                   vars->currentLine,num);
            vars->errorFound = True;
            return INT_MIN;/*error- not an integer*/
        }
    }
    if (strcpy(num,"0")==0)
        number=0; /*atoi doesn't recognize 0 */
    else{
        number=atoi(num);
        if (number==0) /*not an integer*/
        {
            printf("\n%s:Line %d: %s not an integer\n", vars->filename,
                   vars->currentLine,num);
            vars->errorFound = True;
            return INT_MIN;/*error- not an integer*/
        }
    }
    number=number*sign;
    return number;
}

/*by given integer and directive return if the num is in the bit range */
long validNumByDirective(int directive,int num)
{
    int numOfBit=directive*NUMBER_BITS_IN_BYTE;
    int minValue= -1*(pow(2,numOfBit-1));
    int maxValue= (pow(2,numOfBit-1)-1);
    if(minValue<=num && num<=maxValue) /* range of nums [-2^(bitnum-1)....(2^bitnum)-1] */
    {
        return VALID_BIT_RANGE;
    }
    else return INVALID_BIT_RANGE;

}

void ascizAnalysis(char *str,globalVariables *vars)
{
    int valid=isValidString(str);
    if(valid==VALID_STRING)
    {
        for()
    }
    else{
        printf("\n%s:Line %d: %s not a valid string\n", vars->filename,
               vars->currentLine,str);
        vars->errorFound = True;
    }
}


/*to check if a given string is valid, between "" and only graphic characters*/
int isValidString(char *str)
{
    int first=str[0];
    int last=str[strlen(str)-1];
    int i,isGraph;
    if(first!='"'||last!='"' ) return STRING_ERROR; /*a valid string start&end with ""*/

    for(i=0;i< strlen(str);i++)
    {
        isGraph=(int)(str[i]);
        if(isGraph==0) return STRING_ERROR; /*a valid string all the chars are graphic characters*/

    }
    return VALID_STRING;
}


Bool isInstructionCommand(char command[LINE_LENGTH]) {
    return command[0] == '.' ? True : False;
}


