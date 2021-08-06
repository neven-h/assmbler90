//
// Created by ronia on 06/08/2021.
//

#ifndef RONIPROJECT_DIRECTIVEANALYZED_H
#define RONIPROJECT_DIRECTIVEANALYZED_H

#include "default.h"
#include "inputAnalyzed.h"

Bool isDirectiveCommand(char command[LINE_LENGTH]);
int isValidDirectiveName(char *str);

DirectiveWordType getDirectiveType(int directiveNum);
Bool dataAnalysis(char *str,char *before,char *after,globalVariables *vars,int validInput [LINE_LENGTH],int directive);
void ascizAnalysis(char *str,globalVariables *vars);

void labelAndEntryOrExtern(Bool hasLabel,int directiveNum,globalVariables *vars);

#endif //RONIPROJECT_DIRECTIVEANALYZED_H
