//
// Created by ronia on 12/08/2021.
//

#include "printErrors.h"

void printErrors(globalVariables *vars)
{
    if(vars->type==IllegalCharInLabel)
    {
        printf("\n%s:Line %d:Illegal Label found a char that is neither an alphabet nor a digit\n", vars->filename, vars->currentLine);
    }
    if(vars->type==TooLongLabel)
    {
        printf("\n%s:Line %d:Illegal Label more than 31 characters\n", vars->filename,vars->currentLine);
    }
    if(vars->type==firstCharInLabelNotAlphabet)
    {
        printf("\n%s:Line %d:Illegal Label first char not an alphabet\n", vars->filename,vars->currentLine);
    }
    if(vars->type==CommaBeforeFirstParam)
    {
        printf("\n%s:Line %d:Illegal comma before the first param\n", vars->filename,vars->currentLine);
    }
    if(vars->type==CommaBetweenParams)
    {
        printf("\n%s:Line %d:Illegal comma between param\n", vars->filename, vars->currentLine);
    }
    if(vars->type==ParamNotInBitRange)
    {
        printf("\n%s:Line %d:Given number is not in bit range\n", vars->filename, vars->currentLine);
    }
    if(vars->type==notDirectiveOrInstruction)
    {
        printf("\n%s:Line %d:Illegal we couldn't find an Instruction or Directive\n", vars->filename, vars->currentLine);
    }
    if(vars->type==labelExistsInTable)
    {
        printf("\n%s:Line %d: Label already exists in table\n", vars->filename,vars->currentLine);
    }
    if(vars->type==IllegalInstruction)
    {
        printf("\n%s:Line %d: Instruction name is illegal \n", vars->filename, vars->currentLine);
    }
    if(vars->type==IllegalOperandNoComma)
    {
        printf("\n%s:Line %d:Illegal operand no comma\n", vars->filename,vars->currentLine);
    }

    if(vars->type==RegisterLength)
    {
        printf("\n%s:Line %d: register is to long\n", vars->filename, vars->currentLine);
    }
    if(vars->type==RegisterSign)
    {
        printf("\n%s:Line %d: Register must start with $ \n", vars->filename, vars->currentLine);
    }
    if(vars->type==RegisterNegative)
    {
        printf("\n%s:Line %d: Register couldn't be negative \n", vars->filename, vars->currentLine);
    }
    if(vars->type==RegisterNotAnInt)
    {
        printf("\n%s:Line %d: Register couldn't be negative \n", vars->filename, vars->currentLine);
    }
    if(vars->type==RegisterNotInRange)
    {
        printf("\n%s:Line %d: Register must be a number between 0 to 31 \n", vars->filename, vars->currentLine);
    }
    if(vars->type==ExtraneousComma)
    {
        printf("\n%s:Line %d:Extraneous Comma\n", vars->filename,vars->currentLine);
    }
    if(vars->type==ExtraneousText)
    {
        printf("\n%s:Line %d:Illegal operand no comma\n", vars->filename,vars->currentLine);
    }
    if(vars->type==ExtraneousOperand)
    {
        printf("\n%s:Line %d:Extraneous operand \n", vars->filename, vars->currentLine);
    }
    if(vars->type==MissingOperand)
    {
        printf("\n%s:Line %d:Missing Operand\n", vars->filename,vars->currentLine);
    }
    if(vars->type==ImmediateNotAnInt)
    {
        printf("\n%s:Line %d: Immediate must be an integer \n", vars->filename, vars->currentLine);
    }

    if(vars->type==ImmediateNotValid)
    {
        printf("\n%s:Line %d: not a valid Immediate \n", vars->filename, vars->currentLine);
    }
    if(vars->type==ImmediateNotInRange)
    {
        printf("\n%s:Line %d: Immediate must be a number in 16 bits in two's complement range \n", vars->filename, vars->currentLine);
    }
    if(vars->type==InvalidOperand)
    {
        printf("\n%s:Line %d: Invalid Operand\n", vars->filename,vars->currentLine);
    }
    if(vars->type==ExtraneousImmediate)
    {
        printf("\n%s:Line %d:Extraneous Immediate \n", vars->filename, vars->currentLine);
    }
    if(vars->type==DirectiveOperandNotAnInt)
    {
        printf("\n%s:Line %d: Given directive operand is not an integer\n", vars->filename,vars->currentLine);
    }

    if(vars->type==StringNotValid)
    {
        printf("\n%s:Line %d: Given asciiz is not a valid string\n", vars->filename,vars->currentLine);
    }
    if(vars->type==LabelExistsWithoutExternal)
    {
        printf("\n%s:Line %d: Error- Label exists in label list but without External attribute \n", vars->filename,vars->currentLine);
    }
    if(vars->type==LabelExistsInTable)
    {
        printf("\n%s:Line %d: Error- Label already exists in label list \n", vars->filename,vars->currentLine);
    }
    if(vars->type==InvalidTextAfterStop)
    {
        printf("\n%s:Line %d: Extraneous text , stop command doesn't get any operands\n", vars->filename,vars->currentLine);
    }
    if(vars->type==EntryLabelDontExists)
    {
        printf("\n%s:Line %d: Error- Entry Label don't exists in Label Table \n", vars->filename,vars->currentLine);
    }
    if(vars->type==JCommandLabelDontExists)
    {
        printf("\n%s:Line %d: Error- Couldn't find the requested operand label in label list \n", vars->filename,vars->currentLine);
    }
    if(vars->type==IBranchLabelIsExternal)
    {
        printf("\n%s:Line %d: Error- I -Branch label can't be an External label \n", vars->filename,vars->currentLine);
    }
    if(vars->type==IBranchLabelDontExists)
    {
        printf("\n%s:Line %d: Error- Couldn't find the requested operand label in label list \n", vars->filename,vars->currentLine);
    }

    if(vars->type==notDirectiveOrInstruction)
    {
        printf("\n%s:Line %d:Illegal we couldn't find an Instruction or Directive\n", vars->filename, vars->currentLine);
    }


}