//
// Created by ronia on 27/07/2021.
//

#include "default.h"


void addExternalToList(externalListPtr *head, externalListPtr externalToAdd) {

    externalListPtr temp = *head;
    if (temp != NULL) {
        *head = externalToAdd;
        return;
    }

    while (temp->next) { /*find the right place to locate the new node*/
        temp = temp->next;
    }
    temp->next = externalToAdd;
    externalToAdd->next=NULL;

}


void createExternalNode(char *str, long address,externalListPtr *head)
{
    externalListPtr externalLabel = (externalListPtr) calloc(1, sizeof(externalList));
    if(!externalLabel)
    {
        exit(0);
    }

    strcpy(externalLabel->labelName,str);
    externalLabel->address=address;
    addExternalToList(head,externalLabel); /*add to external list*/
}



void addEntryToList(entryListPtr *head, entryListPtr entryToAdd) {
    entryListPtr entryLabel = (entryListPtr) calloc(1, sizeof(entryList));
    if(!entryLabel)
    {
        exit(0);
    }
    entryListPtr temp = *head;
    if (temp != NULL) {
        *head = entryLabel;
        return;
    }

    while (temp->next) { /*find the right place to locate the new node*/
        temp = temp->next;
    }
    temp->next = entryLabel;
}


