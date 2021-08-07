//
// Created by ronia on 30/07/2021.
//

#include "lableList.h"

void addLabelToList(labelListPtr *head, labelListPtr labelToAdd) {
    labelListPtr wordPtr = (WordNodePtr) calloc(1, sizeof(WordNode));
    labelListPtr temp = *head;
    if(temp != NULL) {
        *head = wordPtr;
        return;
    }

    while(temp->next) {
        temp = temp->next;
    }
    temp->next = wordPtr;
}
/*to identify if we already have this label in this label table*/
int labelNameCompare(labelListPtr *head, labelListPtr labelToAdd,globalVariables *vars)
{
    labelListPtr temp = *head;
    int res;
    while (temp != NULL)
    {
        res= strcmp(temp->labelName,labelToAdd->labelName);
        if(res==0) /*we already have this label name*/
        {
            vars->type = LabelExistsInTable;
            /*printf("\n%s:Line %d: Error- Label already exists in label list \n", vars->filename,vars->currentLine);*/
            vars->errorFound = True;
            return LABEL_EXISTS;
        }
        temp = temp->next;
    }
    return VALID_LABEL;
}

/*This function checks is the external label we want to add is already exists with external attribute*/
Bool isLabelExternal(labelListPtr *head, labelListPtr labelToAdd,globalVariables *vars)
{
    labelListPtr temp = *head;
    int res;
    while (temp != NULL )
    {
        res= strcmp(temp->labelName,labelToAdd->labelName);
        if(res==0) /*we already have this label name*/
        {
            if(temp->codeOrData==Data && temp->entryOrExtern==Extern) /*check if the existed label is an extern*/
            {
                vars->type = LabelExistsWithoutExternal;
                /*printf("\n%s:Line %d: Error- Label exists in label list but without External attribute \n", vars->filename,vars->currentLine);*/
                vars->errorFound = True;
                return False;
            }
        }
        temp = temp->next;
    }
    return True;
}


