//
// Created by ronia on 27/07/2021.
//

#ifndef MMN14_INPUTANALYZED_H
#define MMN14_INPUTANALYZED_H

#include "default.h"



void strip(char*);
int isEmptyOrCommandLine(char *);
int split(char *str, char *delimiter, char *before, char *after);
int isLegalLabel(char*str,globalVariables *vars);
int isValidNumberDirective(char *str,globalVariables *vars);

WordType directiveOrInstruction(char *lineCpy,char *before,char *after,globalVariables *vars);

long validNumByDirective(int directive,int num);


int isValidString(char *str);


int Rfunct(int instructionNum);
int instructionValidName(char command[LINE_LENGTH]);
int numberOfOperands(InstructionWordType command,int instructionNum);
int isValidRegisterNum(char *str,globalVariables *vars);
int isValidRegister(char *str,globalVariables *vars);
int isValidImmediate(char *str,globalVariables *vars);
InstructionWordType commandGroup (int instructionNum);
Bool foundLabel(char *lineCpy,char *before,char *after,globalVariables *vars,labelListPtr currentLabel);



#endif //MMN14_INPUTANALYZED_H
