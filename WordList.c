//
// Created by ronia on 30/07/2021.
//

#include "WordList.h"
#include "default.h"

WordNodePtr createWord();

WordNodePtr createWord()
{
    WordNodePtr wordPtr = (WordNodePtr) calloc(1, sizeof(WordNode));
    if(!wordPtr)
    {
        exit(0);
    }
    wordPtr->next=NULL;
    return wordPtr;
}

void addWordToList(WordNodePtr *head, Word word) {
    WordNodePtr wordPtr = (WordNodePtr) calloc(1, sizeof(WordNode));
    if(!wordPtr)
    {
        exit(0);
    }
    WordNodePtr temp = *head;
    fillWord(wordPtr, word);
    if (temp != NULL) {
        *head = wordPtr;
        return;
    }

    while (temp->next) { /*find the right place to locate the new node*/
        temp = temp->next;
    }
    temp->next = wordPtr;
}

void fillWord(WordNodePtr wordPtr, Word word) {
    if (word.wordType == Instruction) {
        wordPtr->word.wordType = Instruction;
        wordPtr->word.instruction.address = word.instruction.address;
        if (word.wordType == R_WORD) {
            wordPtr->word.instruction.wordType = R_WORD;
            wordPtr->word.instruction.rWord.rd = word.instruction.rWord.rd;
            wordPtr->word.instruction.rWord.rs = word.instruction.rWord.rs;
            wordPtr->word.instruction.rWord.rt = word.instruction.rWord.rt;
            wordPtr->word.instruction.rWord.opcode = word.instruction.rWord.opcode;
            wordPtr->word.instruction.rWord.funct = word.instruction.rWord.funct;
            wordPtr->word.instruction.rWord.unused = word.instruction.rWord.unused;
        } else if (word.wordType == I_WORD) {
            wordPtr->word.instruction.wordType = I_WORD;
            wordPtr->word.instruction.iWord.immed = word.instruction.iWord.immed;
            wordPtr->word.instruction.iWord.rt = word.instruction.iWord.rt;
            wordPtr->word.instruction.iWord.rs = word.instruction.iWord.rs;
            wordPtr->word.instruction.iWord.opcode = word.instruction.iWord.opcode;

        } else { /*word.wordType == J_WORD */
            wordPtr->word.instruction.wordType = J_WORD;
            wordPtr->word.instruction.jWord.address = word.instruction.jWord.address;
            wordPtr->word.instruction.jWord.reg = word.instruction.jWord.reg;
            wordPtr->word.instruction.jWord.opcode = word.instruction.jWord.opcode;

        }
    } else { /*word.wordType == Directive*/
        wordPtr->word.wordType = Directive;
        wordPtr->word.directive.address = word.directive.address;
        if (word.wordType == D_BYTE) {
            wordPtr->word.directive.wordType = D_BYTE;
            wordPtr->word.directive.address = word.directive.address;
            wordPtr->word.directive.db = word.directive.db;

        } else { /*directive*/
            if (word.wordType == D_HALF) {
                wordPtr->word.directive.wordType = D_HALF;
                wordPtr->word.directive.address = word.directive.address;
                wordPtr->word.directive.dh = word.directive.dh;
            } else {
                if (word.wordType == D_WORD) {
                    wordPtr->word.directive.wordType = D_WORD;
                    wordPtr->word.directive.address = word.directive.address;
                    wordPtr->word.directive.dw = word.directive.dw;
                } else { /*word.wordType == ASCIZ*/

                    wordPtr->word.directive.wordType = ASCIZ;
                    wordPtr->word.directive.address = word.directive.address;
                    wordPtr->word.directive.asciz = word.directive.asciz;
                }
            }

        }

    }
    wordPtr->next = NULL;
}


void addDirectiveByteToWordList(int validInput[LINE_LENGTH], WordNodePtr *head, int directive,
                                DirectiveWordType givenWordType, int DC) {
    int i;
    if (givenWordType == D_BYTE) {
        for (i = 0; i < LINE_LENGTH; i++) {
            WordNodePtr newNode = (WordNodePtr) calloc(1, sizeof(WordNode)); /*creat new node*/
            newNode->word.wordType = Directive;
            newNode->word.directive.wordType = givenWordType;
            newNode->word.directive.address = DC++;
            newNode->word.directive.db = (long) validInput[i];
            addWordToList(head, newNode->word);
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
                    addWordToList(head, newNode->word);
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
                addWordToList(head, newNode->word);
            }
        }

    }
}

void addDirectiveAsciz(char *str, WordNodePtr *head, int directive,
                                DirectiveWordType givenWordType, int DC)
 {
    int i;
         for (i = 0; i < strlen(str); i++)
         {
             WordNodePtr newNode = (WordNodePtr) calloc(1, sizeof(WordNode)); /*creat new node*/
             newNode->word.wordType = Directive;
             newNode->word.directive.wordType = givenWordType;
             newNode->word.directive.address = DC++;
             newNode->word.directive.asciz =  str[i];
             addWordToList(head, newNode->word);
         }

         /*add the \0 char to end the string*/
         WordNodePtr newNode = (WordNodePtr) calloc(1, sizeof(WordNode)); /*creat new node*/
         newNode->word.wordType = Directive;
         newNode->word.directive.wordType = givenWordType;
        newNode->word.directive.address = (DC+=1);
        newNode->word.directive.asciz =  '\0';
         addWordToList(head, newNode->word);


}

//labelNode.
//next = NULL;
//line = "mov $1,$2,$3";
//int foundLabel = split(line, ":", before, after);
//if(foundLabel == 1) {
//labelNode.
//name = before;
//strip(after);
//split(after,
//" \t", before, after);
//if(

//isDirective (strip(before))

// == True) {
labelNode.
address = globals->dc;
type = getDirectiveType(before);
if(type == Extern)
labelNode.
entryOrExtern =
extern;
else if(type == Entry)
labelNode.
entryOrExtern = entry;
else
labelNode.
entryOrExtern = none;

if(isDataCommand) // if data or code
labelNode.
CodeOrData = data;
else // if this is extern or entry
labelNode.
CodeOrData = none;
} else {
labelNode.
address = globals->ic;
labelNode.
codeOrData = code;
labelNode.
entryOrExtern = none;
}



// .asciz "abcdef"


//char string[100];
//string = getASCIZString();

//for(int i = 0;i < string.length;i++) {
//WordNodePtr word = calloc(1, sizeof(WordNode));
//word.wordType = Directive;
//word.directive.wordType = asciz;
//word.directive.address = DC++;
//word.directive.asci = string[i];
//addWordToList(globals->wordHead, word);
//}

//WordNodePtr word = calloc(1, sizeof(WordNode));
//word.wordType = Directive;
//word.directive.wordType = asciz;
//word.directive.address = DC++;
//word.directive.asci = '\0';
//addWordToList(globals
//->wordHead, word);

//while(1) {

//}
