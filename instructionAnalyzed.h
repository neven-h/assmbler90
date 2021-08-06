//
// Created by ronia on 05/08/2021.
//

#ifndef RONIPROJECT_INSTRUCTIONANALYZED_H
#define RONIPROJECT_INSTRUCTIONANALYZED_H

#include "inputAnalyzed.h"


InstructionWordType commandGroup (int instructionNum);




Bool validROperandLine(char *str,char *before ,char *after, int instructionNum,int numOfOperands,globalVariables *vars, WordNodePtr currentWord,int foundValidOperands);
Bool validIOperandLine(char *str,char *before ,char *after, int instructionNum,int type,globalVariables *vars, WordNodePtr currentWord);

Bool I_commandAnalyzed(char *str,char *before ,char *after, int instructionNum,int type,globalVariables *vars, WordNodePtr currentWord);
Bool R_commandAnalyzed(char *str,char *before ,char *after, int instructionNum,int numOfOperands,globalVariables *vars, WordNodePtr currentWord);



#endif //RONIPROJECT_INSTRUCTIONANALYZED_H
