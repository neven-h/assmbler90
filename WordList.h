//
// Created by ronia on 30/07/2021.
//

#ifndef RONIPROJECT_WORDLIST_H
#define RONIPROJECT_WORDLIST_H

#include "default.h"
#include "inputAnalyzed.h"


void addWordToList(WordNodePtr *head, WordNodePtr nodeToAdd);
void addDirectiveByteToWordList(long validInput[LINE_LENGTH], WordNodePtr *head, int directive, DirectiveWordType givenWordType,globalVariables *vars);
void addDirectiveAsciz(char *str, WordNodePtr *head, DirectiveWordType givenWordType, int DC);
void addDirectiveICF(WordNodePtr *head,int ICF);
void addLabelAddress(WordNodePtr *head,int ICcounter,long labelAddress,InstructionWordType commandType,Bool isExtern);


#endif //RONIPROJECT_WORDLIST_H
