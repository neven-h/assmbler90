//
// Created by ronia on 05/08/2021.
//


#include "default.h"
#include "instructionAnalysed.h"

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
InstructionWordType commandGroup (int instructionNum)
{
    if(instructionNum>=1 &&instructionNum<=8 )
        return R_WORD;
    if(instructionNum>=9 &&instructionNum<=23 )
        return I_WORD;
    if(instructionNum>=24 &&instructionNum<=27 )
        return J_WORD;
}

int numberOfOperands(InstructionWordType command,int instructionNum)
{
    if(command==R_WORD)
    {
        if(1<=instructionNum && instructionNum<=5)
            return THREE_REGISTERS;
        if(6<=instructionNum && instructionNum<=8)
            return TWO_REGISTERS;
    }
    if(command==I_WORD)
    {
        if(9<=instructionNum && instructionNum<=13)
            return REG_IM_REG;
        if(14<=instructionNum && instructionNum<=17)
            return REG_REG_LABEL;
        if(18<=instructionNum && instructionNum<=23)
            return REG_IM_REG;
    }
    if(command==J_WORD)
    {
        if(instructionNum==JMP) /*JMP*/
            return REG_OR_LABEL;
        if(instructionNum==LA ||instructionNum==CALL)
            return ONE_LABEL;
        if(instructionNum==STOP)
            return NONE;
    }
}

Bool RcommandAnalyed(char *str,char *before ,char *after, int instructionNum,int numOfOperands,globalVariables *vars, WordNodePtr currentWord)
{
    int foundValidOperands=0;
    int firstDelimiter,secondDelimiter;
    int firstReg,validReg;
    Bool validOperandLine;

    int funct= Rfunct(numOfOperands);
    currentWord->word.instruction.rWord.funct =funct; /*add funct to current word*/
    validOperandLine=validDelimiter(str,before,after,instructionNum,numOfOperands,vars,currentWord);


}



Bool validDelimiter(char *str,char *before ,char *after, int instructionNum,int numOfOperands,globalVariables *vars, WordNodePtr currentWord,int foundValidOperands)
{
    int validReg;
    int firstReg,secondReg,thirdReg;
    int firstDelimiter,secondDelimiter,thirdDelimiter;

    /*look for the first comma */
    firstDelimiter= split(str,",",before,after);
    if (firstDelimiter==VALID_SPLIT) /*we found he first comma*/
    {
        strip(before);
        if(strlen(before)==0 && foundValidOperands==0) /*the first num is without a ,*/
        {
            vars->type=CommaBeforeFirstParam;
            // printf("\n%s:Line %d:Illegal comma before the first param\n", vars->filename,
            //      vars->currentLine);
            vars->errorFound = True;
            return False;
        }
        /*maybe not relevant for first*/
        if(strlen(before)==0 && foundValidOperands!=0) /*+65,,7...*/
        {
            vars->type=CommaBetweenParams;
            // printf("\n%s:Line %d:Illegal comma between param\n", vars->filename,
            //      vars->currentLine);
            vars->errorFound = True;
            return False;
        }
    }
    else { /*wwe couldn't find the first comma*/
        vars->type=IllegalOperandNoComma;
        // printf("\n%s:Line %d:Illegal operand no comma\n", vars->filename,
        //      vars->currentLine);
        vars->errorFound = True;
        return False;
    }
    /*if we haven't returned so we fount the comma and its valid one, check if a valid register*/
    validReg=isValidRegister(before,vars);
    if(validReg>=0) {
        firstReg = validReg;
        if(numOfOperands==3) {
            currentWord->word.instruction.rWord.rd = firstReg;
        }
        if(numOfOperands==2)
        {
            currentWord->word.instruction.rWord.rs = firstReg;
        }
    }
    /*look for the second comma*/
    secondDelimiter= split(str,",",before,after);
    strip(before);
    strip(after);
    if(secondDelimiter==VALID_SPLIT) /*we found the second comma */
    {
        if(numOfOperands==2)
        {
            if(strcmp(after,"")==0)/*after is an empty string, we just have a comma*/
            {
                vars->type=ExtraneousComma;
                //printf("\n%s:Line %d:Extraneous Comma\n", vars->filename,vars->currentLine);
                vars->errorFound = True;
                return False;
            }
            else{ /*after isn't an empty string - check if a valid operand*/
                validReg=isValidRegister(after,vars);
                if(validReg>=0) /*we found a valid reg so we have more operands*/
                {
                    vars->type=ExtraneousOperand;
                    //printf("\n%s:Line %d:Extraneous operand \n", vars->filename, vars->currentLine);
                    vars->errorFound = True;
                    return False;
                }
                else{ /*if its not a valid reg - Extraneous Text*/
                    vars->type=ExtraneousText;
                    //printf("\n%s:Line %d:Extraneous Text\n", vars->filename,vars->currentLine);
                    vars->errorFound = True;

                }
            }
        }
        if(numOfOperands==3) /*valid situation if we find a second comma*/
        {
            validReg=isValidRegister(before,vars);
            if(validReg>=0) {
                secondReg = validReg;
                currentWord->word.instruction.rWord.rs = secondReg;
            }
            else{ /*not a valid register*/
                return False; /*error type already been checked*/
            }
        }
    }
    else{ /*couldn't find the second comma */
        if(numOfOperands==3) /*for example $3,$2    */
        {
            vars->type=MissingOperand;
            // printf("\n%s:Line %d:Missing Operand\n", vars->filename,vars->currentLine);
            vars->errorFound = True;
            return False;
        }
        if(numOfOperands==2) /*valid situation check if before is a valid register*/
        {
            validReg=isValidRegister(before,vars);
            if(validReg>=0) {
                secondReg = validReg;
                currentWord->word.instruction.rWord.rd = secondReg;
                currentWord->word.instruction.rWord.rt = 0;
                return True;
            }
            else{ /*not a valid register*/
                return False; /*error type already been checked*/
            }
        }
    }
    /*look for the third comma*/
    thirdDelimiter= split(str,",",before,after);
    strip(before);
    strip(after);
    if(thirdDelimiter==VALID_SPLIT) /*if we found the third comma*/
    {
        if(strcmp(after,"")==0)/*after is an empty string, we just have a comma*/
        {
            vars->type=ExtraneousComma;
            //printf("\n%s:Line %d:Extraneous Comma\n", vars->filename,vars->currentLine);
            vars->errorFound = True;
            return False;
        }
        else{ /*after isn't an empty string - check if a valid operand*/
            validReg=isValidRegister(after,vars);
            if(validReg>=0) /*we found a valid reg so we have more operands*/
            {
                vars->type=ExtraneousOperand;
                //printf("\n%s:Line %d:Extraneous operand \n", vars->filename, vars->currentLine);
                vars->errorFound = True;
                return False;
            }
            else{ /*if its not a valid reg - Extraneous Text*/
                vars->type=ExtraneousText;
                //printf("\n%s:Line %d:Extraneous Text\n", vars->filename,vars->currentLine);
                vars->errorFound = True;
            }
        }
    }
    else{ /* we couldn't find the third comma, and we are in a 3 operands situation, so check if the third operand is valid register*/

        validReg=isValidRegister(before,vars);
        if(validReg>=0) {
            thirdReg = validReg;
            currentWord->word.instruction.rWord.rt = thirdReg;
            return True;
        }
        else{ /*not a valid register*/
            return False; /*error type already been checked*/
        }
    }

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
        return REGISTER_ERROR_LENGTH;
    }

    if(str[0]!='$')/*register must begging with $ */
    {
        vars->type=RegisterSign;
        //printf("\n%s:Line %d: Register must start with $ \n", vars->filename, vars->currentLine);
        vars->errorFound = True;
        return REGISTER_ERROR_SIGN;
    }

    strcpy(currentReg,str+1);/*register must be positive number */
    if(currentReg[0]=='-')
    {
        vars->type=RegisterNegative;
        //printf("\n%s:Line %d: Register couldn't be negative \n", vars->filename, vars->currentLine);
        vars->errorFound = True;
        return REGISTER_ERROR_NEGATIVE;
    }

    validNum= isValidRegisterNum(currentReg,vars);
    if(validNum>=0)return validNum;
}

/*a valid register num is an integer between 0-31*/
int isValidRegisterNum(char *str,globalVariables *vars)
{
    int i,num;
    for(i=0;i< strlen(str);i++) /*check if an integer*/
    {
        if(isdigit(str[i])==0)
        {
            vars->type=RegisterNotAnInt;
            //printf("\n%s:Line %d: Register must be an integer \n", vars->filename, vars->currentLine);
            vars->errorFound = True;
            return REG_NOT_INT;
        }
    }
    if(strcmp(str,"0")!=0)
    {
        num=atoi(str); /*atoi returns 0 if its not an int*/
    }
    else{
        num=0;
    }
    if (num<0 || num>31)
    {
        vars->type=RegisterNotInRange;
        //printf("\n%s:Line %d: Register must be a number between 0 to 31 \n", vars->filename, vars->currentLine);
        vars->errorFound = True;
        return REG_NOT_IN_RANGE;
    }
    return num;
}

