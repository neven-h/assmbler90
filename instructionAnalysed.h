//
// Created by ronia on 05/08/2021.
//

#ifndef RONIPROJECT_INSTRUCTIONANALYSED_H
#define RONIPROJECT_INSTRUCTIONANALYSED_H

#include "inputAnalyzed.h"

int instructionValidName(char command[LINE_LENGTH]);
InstructionWordType commandGroup (int instructionNum);

int numberOfOperands(InstructionWordType command,int instructionNum);
int isValidRegisterNum(char *str,globalVariables *vars);
int isValidRegister(char *str,globalVariables *vars);
int Rfunct(int instructionNum);
Bool validDelimiter(char *str,char *before ,char *after, int instructionNum,int numOfOperands,globalVariables *vars, WordNodePtr currentWord);

#endif //RONIPROJECT_INSTRUCTIONANALYSED_H
