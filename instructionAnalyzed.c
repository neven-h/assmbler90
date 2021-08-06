//
// Created by ronia on 05/08/2021.
//


#include "default.h"
#include "instructionAnalyzed.h"
#include "firstPass.h"



Bool R_commandAnalyzed(char *str,char *before ,char *after, int instructionNum,int numOfOperands,globalVariables *vars, WordNodePtr currentWord)
{
    Bool validOperandLine;
    int funct= Rfunct(numOfOperands);
    currentWord->word.instruction.rWord.funct =funct; /*add funct to current word*/

    validOperandLine=validROperandLine(str,before,after,instructionNum,numOfOperands,vars,currentWord);
    if(validOperandLine==True) /*the R command line is valid update the word node and add to word list*/
    {
        currentWord->word.instruction.wordType= R_WORD;
        currentWord->word.instruction.rWord.opcode =0;
        currentWord->word.instruction.rWord.unused =0;
        currentWord->word.instruction.address = vars->IC;
        return True;
    }
    else {return False; }/*not a valid R command line*/

}



Bool validROperandLine(char *str,char *before ,char *after, int instructionNum,int numOfOperands,globalVariables *vars, WordNodePtr currentWord,int foundValidOperands)
{
    int validReg;
    int firstReg,secondReg,thirdReg;
    int firstDelimiter,secondDelimiter,thirdDelimiter;

    /*look for the first comma */
    firstDelimiter= split(str,",",before,after);
    if (firstDelimiter==VALID_SPLIT) /*we found he first comma*/
    {
        strip(before);
        if(strlen(before)==0) /*the first operand is without a comma*/
        {
            vars->type=CommaBeforeFirstParam;
            // printf("\n%s:Line %d:Illegal comma before the first param\n", vars->filename,
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
    secondDelimiter= split(after,",",before,after);
    strip(before);
    strip(after);
    if(secondDelimiter==VALID_SPLIT) /*we found the second comma */
    {
        if(strcmp(before,"")==0) /*$3,,7...*/
        {
                vars->type=CommaBetweenParams;
                // printf("\n%s:Line %d:Illegal comma between param\n", vars->filename,
                //      vars->currentLine);
                vars->errorFound = True;
                return False;
        }

        if(numOfOperands==TWO_REGISTERS)
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
        if(numOfOperands==THREE_REGISTERS) /*valid situation if we find a second comma*/
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
        if(numOfOperands==THREE_REGISTERS) /*for example $3,$2    */
        {
            vars->type=MissingOperand;
            // printf("\n%s:Line %d:Missing Operand\n", vars->filename,vars->currentLine);
            vars->errorFound = True;
            return False;
        }
        if(numOfOperands==TWO_REGISTERS) /*valid situation check if before is a valid register*/
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
    thirdDelimiter= split(after,",",before,after);
    strip(before);
    strip(after);
    if(thirdDelimiter==VALID_SPLIT) /*if we found the third comma*/
    {
        if(strcmp(before,"")==0) /*$3,$2,,$4...*/
        {
            vars->type=CommaBetweenParams;
            // printf("\n%s:Line %d:Illegal comma between param\n", vars->filename,
            //      vars->currentLine);
            vars->errorFound = True;
            return False;
        }

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



Bool I_commandAnalyzed(char *str,char *before ,char *after, int instructionNum,int type,globalVariables *vars, WordNodePtr currentWord)
{
    Bool validOperandLine;
    int opcodeI=instructionNum+1;
    currentWord->word.instruction.iWord.opcode =opcodeI;

    validOperandLine=validIOperandLine(str,before,after,instructionNum,type,vars,currentWord);
    if(validOperandLine==True)
    {
    currentWord->word.instruction.address = vars->IC;
    return True;
    }
    else {return False; }/*not a valid I command line*/

}

Bool validIOperandLine(char *str,char *before ,char *after, int instructionNum,int type,globalVariables *vars, WordNodePtr currentWord)
{
    int validReg,validImmediate;
    int firstReg,secondReg,thirdReg;
    int firstDelimiter,secondDelimiter,thirdDelimiter;

    /*look for the first comma */
    firstDelimiter= split(str,",",before,after);
    {
        if(firstDelimiter==VALID_SPLIT) /*we found a comma check if valid operand*/
        {
            if(strlen(before)==0) /*the first operand is without a comma*/
            {
                vars->type=CommaBeforeFirstParam;
                // printf("\n%s:Line %d:Illegal comma before the first param\n", vars->filename,
                //      vars->currentLine);
                vars->errorFound = True;
                return False;
            }

            /*all I operators starts with register*/
            strip(before);
            validReg=isValidRegister(before,vars);
            if(validReg>=0) {
                firstReg = validReg;
                if(type==REG_REG_LABEL||type==REG_IM_REG_LOAD){
                    currentWord->word.instruction.iWord.rs = firstReg;
                }
                if(type==REG_IM_REG_ARI_LOG)
                {
                    currentWord->word.instruction.iWord.rt = firstReg;
                }
            }
            else{ /*not a valid register*/
                return False; /*error type already been checked*/
            }
        }
        else{
            /*wwe couldn't find the first comma*/
            vars->type=IllegalOperandNoComma;
            // printf("\n%s:Line %d:Illegal operand no comma\n", vars->filename,
            //      vars->currentLine);
            vars->errorFound = True;
            return False;
        }
    }
    /*look for the second comma */
    secondDelimiter= split(after,",",before,after);
    strip(before);
    strip(after);
    if(secondDelimiter==VALID_SPLIT) /*we found the second comma*/
    {
        if (strcmp(before, "") == 0) /*$3,,7...*/
        {
            vars->type = CommaBetweenParams;
            // printf("\n%s:Line %d:Illegal comma between param\n", vars->filename,
            //      vars->currentLine);
            vars->errorFound = True;
            return False;
        }

        if (type == REG_IM_REG_ARI_LOG || type == REG_IM_REG_LOAD) /*supposed to be an immediate*/
        {
            validImmediate = isValidImmediate(before, vars);
            if (validImmediate == INT_MAX) {
                return False;
            }
            currentWord->word.instruction.iWord.immed = validImmediate;
        } else {
            if (type == REG_REG_LABEL) /*supposed to be a register*/
            {
                validReg = isValidRegister(before, vars);
                if (validReg >= 0) {
                    secondReg = validReg;
                    currentWord->word.instruction.iWord.rt = secondReg;
                } else { /*not a valid register*/
                    return False; /*error type already been checked*/
                }
            } else {
                vars->type = InvalidOperand;
                /*printf("\n%s:Line %d: Invalid Operand\n", vars->filename,vars->currentLine);*/
                vars->errorFound = True;
                return False;
            }
        }
    }
    else { /*we couldn't find the second comma*/


        /*TO CHECK IF NECESSARY TO ADD THE CHECK OF THE SECOND OPERAND*/


        vars->type=MissingOperand;
        // printf("\n%s:Line %d:Missing Operand\n", vars->filename,vars->currentLine);
        vars->errorFound = True;
        return False;
    }

    /*look for the third comma*/
    thirdDelimiter= split(after,",",before,after);
    strip(before);
    strip(after);
    if(thirdDelimiter==VALID_SPLIT) /*if we found the third comma*/
    {
        if(strcmp(before,"")==0) /*$3,$2,,$4...*/
        {
            vars->type=CommaBetweenParams;
            // printf("\n%s:Line %d:Illegal comma between param\n", vars->filename,
            //      vars->currentLine);
            vars->errorFound = True;
            return False;
        }

        if(strcmp(after,"")==0)/*after is an empty string, we just have a comma $3,12,$6,  */
        {
            vars->type=ExtraneousComma;
            //printf("\n%s:Line %d:Extraneous Comma\n", vars->filename,vars->currentLine);
            vars->errorFound = True;
            return False;
        }
        else { /*after isn't an empty string - check if a valid operand*/
            validReg = isValidRegister(after, vars);
            if (validReg >= 0) /*we found a valid reg so we have more operands*/
            {
                vars->type = ExtraneousOperand;
                //printf("\n%s:Line %d:Extraneous operand \n", vars->filename, vars->currentLine);
                vars->errorFound = True;
                return False;
            } else { /*check if a valid immediate*/
                validImmediate = isValidImmediate(before, vars);
                if (validImmediate != INT_MAX) { /*valid immediate*/
                    vars->type = ExtraneousImmediate;
                    /*printf("\n%s:Line %d:Extraneous Immediate \n", vars->filename, vars->currentLine);*/
                    vars->errorFound = True;
                    return False;
                } else {
                    /*if its not a valid reg or an immediate - Extraneous Text*/
                    vars->type = ExtraneousText;
                    //printf("\n%s:Line %d:Extraneous Text\n", vars->filename,vars->currentLine);
                    vars->errorFound = True;
                }
            }
        }
    }
    else{ /* we couldn't find the third comma, check if an Label or register or none*/
        strip(before);

        if(type==REG_IM_REG_LOAD ||type==REG_IM_REG_ARI_LOG ) {
            validReg = isValidRegister(before, vars);
            if (validReg >= 0) {
                thirdReg = validReg;
                if (type == REG_IM_REG_LOAD) {
                    currentWord->word.instruction.iWord.rt = thirdReg;
                    return True;
                }
                if (type == REG_IM_REG_ARI_LOG) {
                    currentWord->word.instruction.iWord.rs = thirdReg;
                    return True;
                }
            }
            else{ /*not a valid register*/
                return False; /*error type already been checked*/
            }

        }
            if(type==REG_REG_LABEL)
            {
                /*in the first Pass we put the  address as the current IC in immediate, in the second pass we . need to see what to do if we coldnt fins this label*/
            }

        }

    }


Bool J_commandAnalyzed(char *str,char *before ,char *after, int instructionNum,int numOfOperands,globalVariables *vars, WordNodePtr currentWord)
{
    Bool validOperandLine;
    int funct= Rfunct(numOfOperands);
    currentWord->word.instruction.rWord.funct =funct; /*add funct to current word*/

    validOperandLine=validROperandLine(str,before,after,instructionNum,numOfOperands,vars,currentWord);
    if(validOperandLine==True) /*the R command line is valid update the word node and add to word list*/
    {
        currentWord->word.instruction.wordType= R_WORD;
        currentWord->word.instruction.rWord.opcode =0;
        currentWord->word.instruction.rWord.unused =0;
        currentWord->word.instruction.address = vars->IC;
        return True;
    }
    else {return False; }/*not a valid R command line*/

}



Bool validJOperandLine(char *str,char *before ,char *after, int instructionNum,int numOfOperands,globalVariables *vars, WordNodePtr currentWord,int foundValidOperands)
{שול

}
