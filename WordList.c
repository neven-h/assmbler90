//
// Created by ronia on 30/07/2021.
//

#include "WordList.h"
#include "default.h"




void addWordToList(WordNodePtr *head, WordNodePtr nodeToAdd) {
    WordNodePtr temp = *head;
    if (temp != NULL) {
        *head = nodeToAdd;
        return;
    }
    while (temp->next) { /*find the right place to locate the new node*/
        temp = temp->next;
    }
    temp->next = nodeToAdd;
}


void addDirectiveByteToWordList(int validInput[LINE_LENGTH], WordNodePtr *head, int directive, DirectiveWordType givenWordType, int DC)
{
    int i;
    if (givenWordType == D_BYTE) {
        for (i = 0; i < LINE_LENGTH; i++) {
            WordNodePtr newNode = (WordNodePtr) calloc(1, sizeof(WordNode)); /*creat new node*/
            newNode->word.wordType = Directive;
            newNode->word.directive.wordType = givenWordType;
            newNode->word.directive.address = DC++;
            newNode->word.directive.db = (long) validInput[i];
            addWordToList(head, newNode);
        }

    } else {
        if (givenWordType == D_HALF) {
            {
                for (i = 0; i < LINE_LENGTH; i++) {
                    WordNodePtr newNode = (WordNodePtr) calloc(1, sizeof(WordNode)); /*creat new node*/
                    newNode->word.wordType = Directive;
                    newNode->word.directive.wordType = givenWordType;
                    newNode->word.directive.address = (DC = +2);
                    newNode->word.directive.dh = (long) validInput[i];
                    addWordToList(head, newNode);
                }
            }
        } else {
            /* givenWordType==D_WORD WORD=4 Bytes */
            for (i = 0; i < LINE_LENGTH; i++) {
                WordNodePtr newNode = (WordNodePtr) calloc(1, sizeof(WordNode)); /*creat new node*/
                newNode->word.wordType = Directive;
                newNode->word.directive.wordType = givenWordType;
                newNode->word.directive.address = (DC = +4);
                newNode->word.directive.dw = (long) validInput[i];
                addWordToList(head, newNode);
            }
        }

    }
}

void addDirectiveAsciz(char *str, WordNodePtr *head, int directive, DirectiveWordType givenWordType, int DC)
 {
    int i;
         for (i = 0; i < strlen(str); i++)
         {
             WordNodePtr newNode = (WordNodePtr) calloc(1, sizeof(WordNode)); /*creat new node*/
             newNode->word.wordType = Directive;
             newNode->word.directive.wordType = givenWordType;
             newNode->word.directive.address = DC++;
             newNode->word.directive.asciz =  str[i];
             addWordToList(head, newNode);
         }

         /*add the \0 char to end the string*/
         WordNodePtr newNode = (WordNodePtr) calloc(1, sizeof(WordNode)); /*creat new node*/
         newNode->word.wordType = Directive;
         newNode->word.directive.wordType = givenWordType;
         newNode->word.directive.address = (DC+=1);
         newNode->word.directive.asciz =  '\0';
         addWordToList(head, newNode);


}
/*this function adds after the first pass to each directive node in the word list the final IC value to the address*/
void addDirectiveICF(WordNodePtr *head,int ICF)
{
    WordNodePtr temp = *head;

        while(temp) {
            if(temp->word.wordType == Directive) {
                temp->word.directive.address +=ICF;
            }
            temp = temp->next;
        }

}
