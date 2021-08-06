//
// Created by ronia on 06/08/2021.
//

#ifndef RONIPROJECT_DIRECTIVEANALYZED_H
#define RONIPROJECT_DIRECTIVEANALYZED_H

#include "default.h"

Bool isDirectiveCommand(char command[LINE_LENGTH]);
int isValidDirectiveName(char *str);

DirectiveWordType getDirectiveType(int directiveNum);
void dataAnalysis(char *str,char *before,char *after,globalVariables *vars,int validInput [LINE_LENGTH],int directive);
void ascizAnalysis(char *str,globalVariables *vars);

#endif //RONIPROJECT_DIRECTIVEANALYZED_H
