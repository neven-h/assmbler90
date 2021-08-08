//
// Created by ronia on 08/08/2021.
//

#ifndef RONIPROJECT_SECONDPASS_H
#define RONIPROJECT_SECONDPASS_H



#include "default.h"
#include "lableList.h"
#include "WordList.h"
#include "inputAnalyzed.h"
#include "instructionAnalyzed.h"
#include "directiveAnalyzed.h"

Bool isDirectiveSecondPass(char *before,char after ,globalVariables *vars, Bool hasLabel, labelListPtr currentLabel);


#endif //RONIPROJECT_SECONDPASS_H
