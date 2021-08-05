//
// Created by ronia on 30/07/2021.
//

#ifndef RONIPROJECT_WORDLIST_H
#define RONIPROJECT_WORDLIST_H

#include "default.h"


void addWordToList(WordNodePtr *head, Word word);
WordNodePtr createWord();
void fillWord(WordNodePtr, Word);
void addDirectiveByteToWordList(int validInput[LINE_LENGTH], WordNodePtr *head, int directive,
                                DirectiveWordType givenWordType, int DC);
void addDirectiveAsciz(char *str, WordNodePtr *head, int directive,
                       DirectiveWordType givenWordType, int DC);
#endif //RONIPROJECT_WORDLIST_H
