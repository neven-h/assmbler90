//
// Created by ronia on 30/07/2021.
//

#include "lableList.h"


void addLabelToList(labelListPtr *head, labelListPtr labelToAdd) {
    labelListPtr temp = *head;
    if(temp != NULL) {
        *head = labelToAdd;
        return;
    }

    while(temp->next) {
        temp = temp->next;
    }
    temp->next = labelToAdd;
    labelToAdd->next=NULL;
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

/*This function checks if an entry label exists in the label table and add 'Entry' to the attribute*/
Bool isLabelEntry(labelListPtr *head, char *after,globalVariables *vars)
{
    strip(after);
    labelListPtr temp = *head;
    int res,flag=0;
    while (temp != NULL)
    {
        res= strcmp(temp->labelName,after);
        if(res==0) /*we already have this label name*/
        {
            temp->entryOrExtern=Entry;
            flag=1;
        }
        temp = temp->next;
    }

    if(flag==0)/*we couldn't find this label*/
    {
            vars->type = EntryLabelDontExists;
            /*printf("\n%s:Line %d: Error- Entry Label don't exists in Label Table \n", vars->filename,vars->currentLine);*/
            vars->errorFound = True;
            return False;
    }
    return True;
}
/*this function updates the data labels with final IC value after the first pass*/
void  updateLabelTableICF(labelListPtr *head,int ICF)
{
    labelListPtr temp = *head;

    while(temp) {
        if(temp->codeOrData==Data) {
            temp->address+=ICF;
        }
        temp = temp->next;
    }

}
