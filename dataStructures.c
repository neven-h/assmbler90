//
// Created by ronia on 27/07/2021.
//

#include "default.h"

//creat new node
InstructionWordNode* newWordNode()
{
    InstructionWordNode *newNode;

    newNode = (InstructionWordNode*) calloc(1,sizeof(InstructionWordNode));
    if(!newNode)
    {
        exit(0);
    }
    newNode->next=NULL;
    return newNode;
}

InstructionWordNode*  addToWord(InstructionWordNode *ptr, int num[])
{
    int i;
    for ( i = 0; i <LINE_LENGTH ; i++) {
        ptr=newWordNode();
        ptr->next=NULL;

    }
}

symbolTable* newLable()
{
    symbolTable *newNode;
    newNode=(symbolTable*) calloc(1,sizeof(symbolTable));
    if(!newNode)
    {
        exit(0);
    }
    newNode->next=NULL;
    return newNode;
}






