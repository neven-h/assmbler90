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
int labelNameCompare(labelListPtr *head, labelListPtr labelToAdd)
{
    labelListPtr temp = *head;
    int res;
    while (temp != NULL)
    {
        res= strcmp(temp->labelName,labelToAdd->labelName);
        if(res==0) /*we already have this label name*/
        {
            return LABEL_EXISTS;
        }
        temp = temp->next;
    }
    return VALID_LABEL;

}


