//
// Created by ronia on 24/07/2021.
//

#ifndef RONIPROJ_FIRSTPASS_H
#define RONIPROJ_FIRSTPASS_H

#include "default.h"
#include "lableList.h"
#include "WordList.h"

void firstPass(globalVariables *);
Bool isDirectiveFirstPass(char* before,char* after,globalVariables *vars,Bool hasLabel,labelListPtr currentLabel,WordNodePtr currentWord) ;
Bool isInstructionFirstPass(char *before, char *after, globalVariables *vars, Bool hasLabel, labelListPtr currentLabel,
                            WordNodePtr currentWord, int instructionNum);
#endif //RONIPROJ_FIRSTPASS_H

