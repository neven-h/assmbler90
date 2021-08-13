//
// Created by ronia on 30/07/2021.
//

#ifndef RONIPROJECT_LABLELIST_H
#define RONIPROJECT_LABLELIST_H

#include "default.h"
#include "inputAnalyzed.h"


void updateLabel(labelListPtr currentLabel,int address,Location Data,LabelType Extern);
void addLabelToList(labelListPtr *, labelListPtr );
int labelNameCompare(labelListPtr *head, labelListPtr labelToAdd,globalVariables *vars);
Bool isLabelExternal(labelListPtr *head, labelListPtr labelToAdd,globalVariables *vars);
Bool isLabelEntry(labelListPtr *head, char *after,globalVariables *vars);
void  updateLabelTableICF(labelListPtr *head,int ICF);
long findLabel(labelListPtr *head, char *str,globalVariables *vars,InstructionWordType commandType);
Bool existsLabelExternalIBranch(labelListPtr *head, char *str,globalVariables *vars);
Bool existsLabelExternalJ(labelListPtr *head, char *str,globalVariables *vars);

#endif //RONIPROJECT_LABLELIST_H
