//
// Created by ronia on 13/08/2021.
//

#ifndef RONIPROJECT_ENTRYANDEXTERN_H
#define RONIPROJECT_ENTRYANDEXTERN_H


void addExternalToList(externalListPtr *head, externalListPtr externalToAdd);
void createExternalNode(char *str, long address,externalListPtr *head);

void addEntryToList(entryListPtr *head, entryListPtr entryToAdd);
void createEntryNode(char *str, long address,entryListPtr *head);
#endif //RONIPROJECT_ENTRYANDEXTERN_H
