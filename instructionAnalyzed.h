//
// Created by ronia on 05/08/2021.
//

#ifndef RONIPROJECT_INSTRUCTIONANALYZED_H
#define RONIPROJECT_INSTRUCTIONANALYZED_H

#include "inputAnalyzed.h"


InstructionWordType commandGroup (int instructionNum);




Bool validROperandLine(char *str,char *before ,char *after, int instructionNum,int numOfOperands,globalVariables *vars, WordNodePtr currentWord,int foundValidOperands);
Bool validIOperandLine(char *str,char *before ,char *after, int instructionNum,int type,globalVariables *vars, WordNodePtr currentWord);
Bool validJOperandLine(char *str, int instructionNum,globalVariables *vars, WordNodePtr currentWord);

Bool I_commandAnalyzed(char *str,char *before ,char *after, int instructionNum,int type,globalVariables *vars, WordNodePtr currentWord);
Bool R_commandAnalyzed(char *str,char *before ,char *after, int instructionNum,int numOfOperands,globalVariables *vars, WordNodePtr currentWord);
Bool J_commandAnalyzed(char *str, int instructionNum,globalVariables *vars, WordNodePtr currentWord);

Bool regJCommand(char *str,globalVariables *vars, WordNodePtr currentWord);
Bool labelJCommand(char *str,globalVariables *vars, WordNodePtr currentWord);

int opcodeJ(int instructionNum);

Bool secondPassJ(char *str,globalVariables *vars,int ICcounter, InstructionWordType commandType);
Bool secondPassI(char *str,globalVariables *vars,int ICcounter, InstructionWordType commandType);

#endif //RONIPROJECT_INSTRUCTIONANALYZED_H
