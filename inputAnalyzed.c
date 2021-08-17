//
// Created by ronia on 27/07/2021.
//

#include "inputAnalyzed.h"


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
                        // printf("\n%s:Line %d:Illegal Label char %c neither an alphabet nor a digit\n", vars->filename,vars->currentLine, str[i]);
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


int isEmptyOrCommandLine(char *str) {
    if (str[0] == ';'||isspace(str[0])!=0 )
        return 1;
    else return -1;
}



int isValidNumberDirective(char *str,globalVariables *vars)
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
            vars->type=DirectiveOperandNotAnInt;
            /*printf("\n%s:Line %d: %s not an integer\n", vars->filename,vars->currentLine,num);*/
            vars->errorFound = True;
            return INT_MIN;/*error- not an integer*/
        }
    }
    if (strcmp(num,"0")==0)
        number=0; /*atoi doesn't recognize 0 */
    else{
        number=atoi(num);
        if (number==0) /*not an integer*/
        {
            vars->type=DirectiveOperandNotAnInt;
            /*printf("\n%s:Line %d: %s not an integer\n", vars->filename,vars->currentLine,num);*/
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

/*to check if a given string is valid, between "" and only graphic characters*/
int isValidString(char *str)
{
    int first=(int)str[0];
    int last=(int)str[strlen(str)-1];
    int i,isPrint;
    if(first!='"'||last!='"' ) return STRING_ERROR; /*a valid string start&end with ""*/

    for(i=0;i< strlen(str);i++)
    {
        isPrint=(isprint(str[i]));
        if(isPrint==0) return STRING_ERROR; /*a valid string all the chars are graphic characters*/
    }
    return VALID_STRING;
}


void ascizStr(char *str)
{
    int first=1;
    int last=strlen(str)-1;
    char newStr[LINE_LENGTH]={0};
    strncpy(newStr, str + first, last - first );
    memset(str, 0, LINE_LENGTH);
    strcpy(str, newStr);

}

Bool isInstructionCommand(char command[LINE_LENGTH]) {
    return command[0] == '.' ? True : False;
}



int instructionValidName(char command[LINE_LENGTH]) {
    if (strcmp(command, "add") == 0) return ADD;
    if (strcmp(command, "sub") == 0) return SUB;
    if (strcmp(command, "and") == 0) return AND;
    if (strcmp(command, "or") == 0) return OR;
    if (strcmp(command, "nor") == 0) return NOR;
    if (strcmp(command, "move") == 0) return MOVE;
    if (strcmp(command, "mvhi") == 0) return MVHI;
    if (strcmp(command, "mvlo") == 0) return MVLO;
    if (strcmp(command, "addi") == 0) return ADDI;
    if (strcmp(command, "subi") == 0) return SUBI;
    if (strcmp(command, "andi") == 0) return ANDI;
    if (strcmp(command, "ori") == 0) return ORI;
    if (strcmp(command, "nori") == 0) return NORI;
    if (strcmp(command, "bne") == 0) return BNE;
    if (strcmp(command, "beq") == 0) return BEQ;
    if (strcmp(command, "blt") == 0) return BLT;
    if (strcmp(command, "bgt") == 0) return BGT;
    if (strcmp(command, "lb") == 0) return LB;
    if (strcmp(command, "sb") == 0) return SB;
    if (strcmp(command, "lw") == 0) return LW;
    if (strcmp(command, "sw") == 0) return SW;
    if (strcmp(command, "lh") == 0) return LH;
    if (strcmp(command, "sh") == 0) return SH;
    if (strcmp(command, "jmp") == 0) return JMP;
    if (strcmp(command, "la") == 0) return LA;
    if (strcmp(command, "call") == 0) return CALL;
    if (strcmp(command, "stop") == 0) return STOP;
    else return INSTRUCTION_ERROR;
}
/*returns the funct of a given R command*/
int Rfunct(int instructionNum)
{
    if(instructionNum==ADD)
        return ADD_FUNCT; /*command add funct 1*/
    if(instructionNum==SUB)
        return SUB_FUNCT;/*command sub funct 2*/
    if(instructionNum==AND)
        return AND_FUNCT; /*command and funct 3*/
    if(instructionNum==OR)
        return OR_FUNCT; /*command or funct 4*/
    if(instructionNum==NOR)
        return NOR_FUNCT; /*command nor funct 5*/
    if(instructionNum==MOVE)
        return MOVE_FUNCT; /*command move funct 1*/
    if(instructionNum==MVHI)
        return MVHI_FUNCT;/*command mvhi funct 2*/
    if(instructionNum==MVLO)
        return MVLO_FUNCT; /*command mvlo funct 3*/
}
/*by given instruction number returns the instruction word type*/
InstructionWordType commandGroup (int instructionNum)
{
    if(instructionNum>=1 &&instructionNum<=8 )
        return R_WORD;
    if(instructionNum>=9 &&instructionNum<=23 )
        return I_WORD;
    if(instructionNum>=24 &&instructionNum<=27 )
        return J_WORD;
}

/*by given instruction number and command type returns the num of expected operands*/
int numberOfOperands(InstructionWordType command,int instructionNum) {
    int numOfOperands;
    if (command == R_WORD) {
        if (ADD <= instructionNum && instructionNum <= 5)
            numOfOperands=THREE_REGISTERS;
        if (6 <= instructionNum && instructionNum <= 8)
            numOfOperands=TWO_REGISTERS;
    } else {
        if (command == I_WORD) {
            /*all I commands gets 3 operand so we will define by type*/
            if (ADDI <= instructionNum && instructionNum <= 13)
                numOfOperands=REG_IM_REG_ARI_LOG; /*arithmetic and logic - will be 4*/
            if (14 <= instructionNum && instructionNum <= 17)
                numOfOperands=REG_REG_LABEL;
            if (18 <= instructionNum && instructionNum <= 23)
                numOfOperands=REG_IM_REG_LOAD; /*load and store in the memory - will be 6*/
        } else { /*command==J_WORD */
            if (instructionNum == JMP) /*JMP*/
                numOfOperands=REG_OR_LABEL;
            if (instructionNum == LA || instructionNum == CALL)
                numOfOperands=ONE_LABEL;
            if (instructionNum == STOP)
                numOfOperands=NONE;

        }
    }
    return numOfOperands;
}


/*a valid register stars with $ and between 0-31*/
int isValidRegister(char *str,globalVariables *vars)
{
    char currentReg[4]={0};
    int validNum;
    strip(str);
    if(strlen(str)>4) {/*check while debug if in length \0 include*/
        vars->type=RegisterLength;
        //printf("\n%s:Line %d: register is to long\n", vars->filename, vars->currentLine);
        vars->errorFound = True;
        return REGISTER_ERROR;
    }

    if(str[0]!='$')/*register must begging with $ */
    {
        vars->type=RegisterSign;
        //printf("\n%s:Line %d: Register must start with $ \n", vars->filename, vars->currentLine);
        vars->errorFound = True;
        return REGISTER_ERROR;
    }

    strcpy(currentReg,str+1);/*register must be positive number */
    if(currentReg[0]=='-')
    {
        vars->type=RegisterNegative;
        //printf("\n%s:Line %d: Register couldn't be negative \n", vars->filename, vars->currentLine);
        vars->errorFound = True;
        return REGISTER_ERROR;
    }

    validNum= isValidRegisterNum(currentReg,vars);
    if(validNum>=0)return validNum;
}

/*a valid register num is an integer between 0-31*/
int isValidRegisterNum(char *str,globalVariables *vars)
{
    int i,num;
    char reg[10]={0};
    strcpy(reg,str); /*copy to a new string without $ char*/
    for(i=0;i< strlen(reg);i++) /*check if an integer*/
    {
        if(isdigit(reg[i])==0)
        {
            vars->type=RegisterNotAnInt;
            //printf("\n%s:Line %d: Register must be an integer \n", vars->filename, vars->currentLine);
            vars->errorFound = True;
            return REGISTER_ERROR;
        }
    }
    if(strcmp(reg,"0")!=0)
    {
        num=atoi(reg); /*atoi returns 0 if its not an int*/
    }
    else{
        num=0;
    }
    if (num<MIN_REG || num>MAX_REG)
    {
        vars->type=RegisterNotInRange;
        //printf("\n%s:Line %d: Register must be a number between 0 to 31 \n", vars->filename, vars->currentLine);
        vars->errorFound = True;
        return REGISTER_ERROR;
    }
    return num;
}


int isValidImmediate(char *str,globalVariables *vars) {

    char positive[LINE_LENGTH]={0};
    char negative[LINE_LENGTH]={0};

    int i;
    int validNum = 0;
    int minRange= pow(-2,15);
    int maxRange= (pow(2,15)-1);
    int sign = 1;
    if (str[0] == '-') sign = -1;   /*immediate can be a negative number*/

    if (str[0] == '-' || isdigit(str[0]) != 0) {
        for (i = 1; i < strlen(str); i++) /*check if an integer*/
        {
            if (isdigit(str[i]) == 0) {
                vars->type = ImmediateNotAnInt;
                //printf("\n%s:Line %d: Immediate must be an integer \n", vars->filename, vars->currentLine);
                vars->errorFound = True;
                return INT_MAX;
            }
        }
    }
    else {
        vars->type = ImmediateNotValid;
        //printf("\n%s:Line %d: not a valid Immediate \n", vars->filename, vars->currentLine);
        vars->errorFound = True;
        return INT_MAX;
    }

    /*check if the num is in the correct range 16 bits with negative [-2^15...(2^15)-1]*/

    if (str[0] == '-') {
        sign = -1;   /*immediate can be a negative number*/
        strcpy(negative, str + 1);
        if (strcmp(negative, "0") != 0) {
            validNum = atoi(negative); /*atoi returns 0 if its not an int ,and we cant have a negative zero*/
        }
        validNum=sign*validNum;
        if (validNum < minRange || validNum > maxRange) {
            vars->type = ImmediateNotInRange;
            //printf("\n%s:Line %d: Immediate must be a number in 16 bits in two's complement range \n", vars->filename, vars->currentLine);
            vars->errorFound = True;
            return INT_MAX;
        }
        return validNum;
    }
    else{ /*if its not a - it a digit -already checked*/
        strcpy(positive, str);
        if (strcmp(positive, "0") != 0) {
            validNum = atoi(positive); /*atoi returns 0 if its not an int*/
        } else {
            validNum = 0;
        }
        if (validNum < minRange || validNum > maxRange) {
            vars->type = ImmediateNotInRange;
            //printf("\n%s:Line %d: Immediate must be a number in 16 bits in two's complement range \n", vars->filename, vars->currentLine);
            vars->errorFound = True;
            return INT_MAX;
        }
        return validNum;
    }
}

Bool foundLabel(char *lineCpy,char *before,char *after,globalVariables *vars,labelListPtr currentLabel) {
    int labelDelimiter, validLabel;

    labelDelimiter = split(lineCpy, ":", before, after);
    if (labelDelimiter == VALID_SPLIT)//*we found a ':' - Label*/
    {
        validLabel = isLegalLabel(before, vars);
        /*to find if we have a label*/

        if (validLabel == VALID_LABEL) {
            strip(before);
            strcpy(currentLabel->labelName, before);
            return True; /*label flag*/
        }
        else return False; /*we found a label but it's not valid*/
    }
    else return False; /*we couldn't find ":"*/
}

WordType directiveOrInstruction(char *str,char *before,char *after,globalVariables *vars)
{
    WordType word;
    int lineAnalyzed,directiveName,instructionNum;
    Bool isDirective;
    strip(after);
    instructionNum=instructionValidName(after);
    lineAnalyzed = split(str, " \t", before, after);
    if (lineAnalyzed == VALID_SPLIT) { /*we found a tab*/
        strip(before);
        strip(after);
        isDirective =isDirectiveCommand(before); /*if we find a '.' it's a directive*/
        if (isDirective == True) { /*check if a valid directive*/
            directiveName=isValidDirectiveName(before);
            if(directiveName!=DIRECTIVE_ERROR) /*it's a valid directive*/
            {
                word=Directive;
            }
            else {word=None;}
        }
        else { /*check if instruction or non*/
            instructionNum=instructionValidName(before);
            if(instructionNum!=INSTRUCTION_ERROR) /*we found a valid instruction*/
            {
                word=Instruction;
            }
            else{ /*if it's not a directive and not an instruction - error*/
                vars->type=notDirectiveOrInstruction;
                // printf("\n%s:Line %d:Illegal we couldn't find an Instruction or Directive\n", vars->filename,
                //       vars->currentLine);
                vars->errorFound=True;
                word=None;
            }
        }
    }
    else{
        if (instructionNum==STOP)
        {
            word=Instruction;
        }
        else {
            vars->type = notDirectiveOrInstruction;
            // printf("\n%s:Line %d:Illegal we couldn't find an Instruction or Directive\n", vars->filename, vars->currentLine);
            vars->errorFound = True;
            word = None;
        }

    }
    return word;

}


/*this function check if the operand of J is a valid Register*/
int validJRegister(char *str,globalVariables *vars)
{
    char currentReg[4]={0};
    strip(str);
    if(strlen(str)>4) {/*check while debug if in length \0 include*/
        return REGISTER_ERROR;
    }

    if(str[0]!='$')/*register must begging with $ */
    {
        return REGISTER_ERROR;
    }

    strcpy(currentReg,str+1);/*register must be positive number */
    if(currentReg[0]=='-')
    {
        return REGISTER_ERROR;
    }


    return VALID_REGISTER;
}



void resetStrings(char *line,char *lineCpy,char *before , char *after, char *lineCpyAfterLabel,char *label)
{
    memset(line, 0, LINE_LENGTH);
    memset(lineCpy, 0, LINE_LENGTH);
    memset(before, 0, LINE_LENGTH);
    memset(after, 0, LINE_LENGTH);
    memset(lineCpyAfterLabel, 0, LINE_LENGTH);
    memset(label, 0, LABEL_LENGTH);
}