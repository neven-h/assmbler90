//
// Created by ronia on 27/07/2021.
//

#include "default.h"


void addExternalToList(externalListPtr *head, externalListPtr externalToAdd) {
    externalListPtr externalLabel = (externalListPtr) calloc(1, sizeof(externalList));
    if(!externalLabel)
    {
        exit(0);
    }
    externalListPtr temp = *head;
    if (temp != NULL) {
        *head = externalLabel;
        return;
    }

    while (temp->next) { /*find the right place to locate the new node*/
        temp = temp->next;
    }
    temp->next = externalLabel;
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


