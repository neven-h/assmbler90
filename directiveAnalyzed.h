//
// Created by ronia on 06/08/2021.
//

#ifndef RONIPROJECT_DIRECTIVEANALYZED_H
#define RONIPROJECT_DIRECTIVEANALYZED_H

#include "default.h"
#include "inputAnalyzed.h"

Bool isDirectiveFirstPass(char* before,char* after,char* label,globalVariables *vars,Bool hasLabel,labelListPtr currentLabel,WordNodePtr currentWord) ;
Bool isDirectiveSecondPass(char *before,char* after ,globalVariables *vars, Bool hasLabel, labelListPtr currentLabel);

Bool isDirectiveCommand(char command[LINE_LENGTH]);
int isValidDirectiveName(char *str);


DirectiveWordType getDirectiveType(int directiveNum);
Bool dataAnalysis(char *str,char *before,char *after,globalVariables *vars,long validInput [LINE_LENGTH],int directive);
Bool ascizAnalysis(char *str,globalVariables *vars);
Bool externDirectiveFirstPass(char *after, globalVariables *vars,labelListPtr currentLabel);
Bool labelBeforeDirectiveCommand(char *labelName, globalVariables *vars, labelListPtr currentLabel);

void labelAndEntryOrExtern(Bool hasLabel,int directiveNum,globalVariables *vars);

#endif //RONIPROJECT_DIRECTIVEANALYZED_H
