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
int isValidDirectiveName(char *str);
Bool isDirectiveCommand(char command[LINE_LENGTH]);
void dataAnalysis(char *str,char *before,char *after,globalVariables *vars,int validInput [LINE_LENGTH],int directive);
void ascizAnalysis(char *str,globalVariables *vars);
int isValidNumber(char *str,globalVariables *vars);
long validNumByDirective(int directive,int num);
int isValidString(char *str);
DirectiveWordType getDirectiveType(int directiveNum);





#endif //MMN14_INPUTANALYZED_H
