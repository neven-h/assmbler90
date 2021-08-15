//
// Created by ronia on 05/08/2021.
//



#include "instructionAnalyzed.h"


Bool isInstructionFirstPass(char *before, char *after,char *label,globalVariables *vars, Bool hasLabel, labelListPtr currentLabel,WordNodePtr currentWord, int instructionNum) {

    int numOfOperands;
    Bool validRCommand, validICommand,labelBeforeInstruction;

    currentWord->word.wordType = Instruction;
    if (hasLabel == True) {
        labelBeforeInstruction=labelBeforeInstructionCommand(label,vars,currentLabel);
        if(labelBeforeInstruction==False) return False; /*and get the next row, else continue */
    }

    if (instructionNum < ADD || instructionNum > STOP) {
        vars->type = IllegalInstruction;
        // printf("\n%s:Line %d: Instruction name is illegal \n", vars->filename,vars->currentLine);
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
            currentWord->word.instruction.address = vars->IC;
            vars->IC = (vars->IC + 4);
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
            currentWord->word.instruction.address = vars->IC;
            vars->IC = (vars->IC + 4);

            return True;
        } else {
            return False; /*not valid I Command*/
        }
    }
    if (commandType == J_WORD) {
        currentWord->word.instruction.wordType = J_WORD;
        int type = numberOfOperands(commandType, instructionNum);
        strip(after);
        validICommand = J_commandAnalyzed(after, instructionNum, vars, currentWord);
        if (validICommand == True) {
            /*need to add the word node to the list*/
            currentWord->word.instruction.address = vars->IC;
            vars->IC = (vars->IC + 4);
            return True;
        } else {
            return False; /*not valid J Command*/
        }
    }
}


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



Bool validROperandLine(char *str,char *before ,char *after, int instructionNum,int numOfOperands,globalVariables *vars, WordNodePtr currentWord)
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
                else{ /*type == REG_IM_REG_ARI_LOG*/
                    currentWord->word.instruction.iWord.rs = thirdReg;
                    return True;
                }
            }
            else{ /*not a valid register*/
                return False; /*error type already been checked*/
            }
        }
        else{
                if(type==REG_REG_LABEL) /*in the first Pass we put the address as the current IC in immediate, will be handle in second Pass*/
              {
                /*in the first Pass we put the address as the current IC in immediate, will be handle in second Pass*/
                currentWord->word.instruction.iWord.immed=vars->IC;
                return True;
             }
        }
    }
}


Bool J_commandAnalyzed(char *str, int instructionNum,globalVariables *vars, WordNodePtr currentWord)
{
    Bool validOperandLine;
    int opcode=opcodeJ(instructionNum);
    currentWord->word.instruction.jWord.opcode =opcode;

    validOperandLine= validJOperandLine(str,instructionNum,vars,currentWord);

    if(validOperandLine==True)
    {
        return True;
    }
    else {return False; }/*not a valid J command line*/
}


int opcodeJ(int instructionNum)
{
    if(instructionNum==JMP)
        return OP_JMP;
    if(instructionNum==LA)
        return OP_LA;
    if(instructionNum==CALL)
        return OP_CALL;
    if(instructionNum==STOP)
        return OP_STOP;
}


Bool validJOperandLine(char *str, int instructionNum,globalVariables *vars, WordNodePtr currentWord)
{
    strip(str);
    int isReg, isLabel;
    Bool JwithReg, JwithLabel;
    if (instructionNum == JMP) /* jmp cen receive a register or label*/
    {
        isReg = isValidRegister(str, vars); /*check if a register*/
        if (isReg == VALID_REGISTER) {
            JwithReg = regJCommand(str, vars, currentWord);
            if (JwithReg == False)return False;
            /*than True it's a valid register*/
            return True;
        } else { /*not a register - check if a valid label by syntax. in the second pass we will check in the Label Table*/

            JwithLabel = labelJCommand(str, vars, currentWord);
            if (JwithLabel == False) /* not a register and not a label by syntax - operand error*/
            {
                vars->type = InvalidOperand;
                /* printf("\n%s:Line %d: Invalid Operand\n", vars->filename,vars->currentLine); */
                vars->errorFound = True;
                return False;
            } else { return True; /*it's a valid label by syntax*/}
        }
    } else {
        if (instructionNum == LA || instructionNum == CALL) {
            JwithLabel = labelJCommand(str, vars, currentWord);
            if (JwithLabel == False) /* not a register and not a label by syntax - operand error*/
            {
                vars->type = InvalidOperand;
                /* printf("\n%s:Line %d: Invalid Operand\n", vars->filename,vars->currentLine); */
                vars->errorFound = True;
                return False;
            }
        } else {
            if (instructionNum == STOP) { /*stop doesn't get any operands*/
                if(strcmp(str,"")!=0) /*we found text after stop command*/
                {
                    vars->type = InvalidTextAfterStop;
                    /* printf("\n%s:Line %d: Extraneous text , stop command doesn't get any operands\n", vars->filename,vars->currentLine); */
                    vars->errorFound = True;
                    return False;
                }
                currentWord->word.instruction.jWord.reg=0;
                currentWord->word.instruction.jWord.address =0;
                return True;
            }
        }
    }

}

Bool regJCommand(char *str,globalVariables *vars, WordNodePtr currentWord)
{
    int regNum;
    regNum= isValidRegisterNum(str,vars);
    if(regNum<0)/* not valid reg num */
        return False;
    currentWord->word.instruction.jWord.reg=1;
    currentWord->word.instruction.jWord.address =regNum;
    return True;
}


Bool labelJCommand(char *str,globalVariables *vars, WordNodePtr currentWord)
{
   int isLabel;
   isLabel=isLegalLabel(str,vars);
   if(isLabel==LABEL_ERROR) /*not a valid label*/
       return False;
    /*than a valid label - by syntax. in the second pass we will update the address. */
    currentWord->word.instruction.jWord.reg=0;
    return True;
}


Bool secondPassJ(char *str,globalVariables *vars,int ICcounter, InstructionWordType commandType)
{
    int validRegister;
    long labelAddress;
    Bool isExtern;
    /*all J instructions (beside stop) have one operand*/
    strip(str);
    validRegister=isValidRegister(str,vars); /*jmp can get a register*/
    if (validRegister==REGISTER_ERROR) /*not a register so it's a label*/
    {
       /*we need to find the label in label list and update the address*/
        labelAddress= findLabel(&(vars->headLabelTable),str,vars,commandType);
        if (labelAddress==LABEL_ERROR) /*couldn't find the label*/
            return False;
        isExtern= existsLabelExternalJ(&(vars->headLabelTable),str,vars);
        addLabelAddress(&(vars->headWordList),ICcounter,labelAddress,commandType,isExtern);
        if (isExtern==True) /*if it is a J command and the label is extern add to extern list*/
        {
            createExternalNode(str,ICcounter,(&(vars->headExternList)));
        }
        return True;
    }
    return False;
}



Bool secondPassI(char *str,globalVariables *vars,int ICcounter, InstructionWordType commandType) {
    int firstSplit, secondSplit;
    long currentLabelAddress;
    Bool isExternal;
    /*in I-Branch the label is the third operand*/
    strip(str);
    char before[LINE_LENGTH] = {0};
    char after[LINE_LENGTH] = {0};

    firstSplit = split(str, ",", before, after);
    if (firstSplit == INVALID_SPLIT) {
        return False;
    }

    secondSplit = split(after, ",", before, after);
    if (secondSplit == INVALID_SPLIT) /*after second split the label will be in after string*/
    {
        return False;
    }
    /*after second split the label will be in after string*/
    strip(after);
    /*look for branch label in label list*/
    currentLabelAddress = findLabel(&(vars->headLabelTable), str, vars, commandType);
    if (currentLabelAddress == LABEL_ERROR) /*couldn't find the label*/
        return False;
    isExternal = existsLabelExternalIBranch(&(vars->headLabelTable), after, vars);
    if (isExternal == False) /*the I Branch label is external - error*/
        return False;
    addLabelAddress(&(vars->headWordList), ICcounter, currentLabelAddress, commandType,isExternal);
    return True;

}



/*This function handles cases where there is a label before instruction command. check if the label is already in the label list and if not update the label list*/
Bool labelBeforeInstructionCommand(char *labelName, globalVariables *vars, labelListPtr currentLabel)
{
    int ValidLabelName;
    ValidLabelName = labelNameCompare(&(vars->headLabelTable),labelName,vars); /*check if the label name wasn't shown in the table already*/
    if (ValidLabelName == VALID_LABEL) { /* a label isn't in the table*/
        updateLabel(currentLabel,vars->IC,Code,NoEntryExtern); /*update the current label node*/
        addLabelToList((&vars->headLabelTable), currentLabel);/*add the label to table*/
        return True;
    }

    else{ return False;  /*we found the label in the label table*/}
}