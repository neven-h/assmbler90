#ifndef DEFAULT_H
#define DEFAULT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define LINE_LENGTH 82
#define LAST_CHAR_IN_LINE 80
#define FILE_NAME_LENGTH 30
#define LABEL_LENGTH 31
#define AS_EXTENSION_LENGTH 3


#define DIRECTIVE_WORD 4
#define DIRECTIVE_HALF_WORD 2
#define DIRECTIVE_BYTE 1
#define DIRECTIVE_ASCIZ 3
#define DIRECTIVE_EXTERN 5
#define DIRECTIVE_ENTRY 6
#define DIRECTIVE_ERROR -1

#define STRING_ERROR -1
#define VALID_STRING 1

#define LABEL_ERROR -1
#define VALID_LABEL 1
#define LABEL_EXISTS -1

#define VALID_SPLIT 1
#define INVALID_SPLIT -1

#define REGISTER_ERROR_SIGN -1
#define REGISTER_ERROR_LENGTH -2
#define REGISTER_ERROR_NEGATIVE -3
#define REG_NOT_INT -1
#define REG_NOT_IN_RANGE -4

#define VALID_REGISTER 1
#define IMMEDIATE_NOT_INT -1

#define NUMBER_BITS 32
#define NUMBER_BITS_IN_BYTE 8
#define VALID_BIT_RANGE 1
#define INVALID_BIT_RANGE -1


#define ADD 1
#define SUB 2
#define AND 3
#define OR 4
#define NOR 5
#define MOVE 6
#define MVHI 7
#define MVLO 8
#define ADDI 9
#define SUBI 10
#define ANDI 11
#define ORI 12
#define NORI 13
#define BNE 14
#define BEQ 15
#define BLT 16
#define BGT 17
#define LB 18
#define SB 19
#define LW 20
#define SW 21
#define LH 22
#define SH 23
#define JMP 24
#define LA 25
#define CALL 26
#define STOP 27
#define INSTRUCTION_ERROR -1

#define MIN_REG 0
#define MAX_REG 31

#define ADD_FUNCT 1
#define SUB_FUNCT 2
#define AND_FUNCT 3
#define OR_FUNCT 4
#define NOR_FUNCT 5
#define MOVE_FUNCT 1
#define MVHI_FUNCT 2
#define MVLO_FUNCT 3
#define OP_JMP 30
#define OP_LA 31
#define OP_CALL 32
#define OP_STOP 63

#define THREE_REGISTERS 3
#define TWO_REGISTERS 2
#define REG_IM_REG_ARI_LOG  4
#define REG_REG_LABEL 5
#define REG_IM_REG_LOAD  6

#define REG_OR_LABEL 1
#define ONE_LABEL 1
#define NONE 0




typedef enum {Instruction, Directive,None} WordType;
typedef enum {R_WORD, I_WORD, J_WORD} InstructionWordType;
typedef enum {D_BYTE, D_HALF, D_WORD, ASCIZ} DirectiveWordType;
typedef enum {False = 0, True = 1} Bool;
typedef enum {Code, Data } Location;
typedef enum {Entry, Extern, NoEntryExtern} LabelType;
typedef enum {NoError,LineTooLong,IllegalCharInLabel,TooLongLabel,firstCharInLabelNotAlphabet,
              CommaBeforeFirstParam,CommaBetweenParams,ParamNotInBitRange,labelBeforeEntry,labelBeforeExtern,notDirectiveOrInstruction,labelExistsInTable,
              IllegalInstruction,IllegalOperandNoComma,RegisterLength,RegisterSign,RegisterNegative,RegisterNotAnInt,RegisterNotInRange,ExtraneousComma,ExtraneousText,
              ExtraneousOperand,MissingOperand,ImmediateNotAnInt,ImmediateNotValid,ImmediateNotInRange,InvalidOperand,ExtraneousImmediate,DirectiveOperandNotAnInt,StringNotValid,
              LabelExistsWithoutExternal,LabelExistsInTable,InvalidTextAfterStop,EntryLabelDontExists,

              Valid = 100} errorType; /*add error each time, at the end of firstPass - print*/


typedef struct Rfunc {
    unsigned int unused:6;
    unsigned int funct:5;
    unsigned int rd:5;
    unsigned int rt:5;
    unsigned int rs:5;
    unsigned int opcode:6;
} R_cmd;

typedef struct Ifunc {
    unsigned int immed:16;
    unsigned int rt:5;
    unsigned int rs:5;
    unsigned int opcode:6;
} I_cmd;

typedef struct Jfunc {
    unsigned long address:25;
    unsigned int reg:1;
    unsigned int opcode:6;
} J_cmd;

typedef struct InstructionWord {
    InstructionWordType wordType;
    unsigned long address;
    union {
        R_cmd rWord;
        I_cmd iWord;
        J_cmd jWord;
    };
} InstructionWord;

typedef struct DirectiveWord {
    DirectiveWordType wordType;
    unsigned long address;
    union {
        long db:8;
        long dh:16;
        long dw:32;
        char asciz;
    };
} DirectiveWord;

typedef struct Word {
    WordType wordType;
    union {
        InstructionWord instruction;
        DirectiveWord directive;
    };
} Word;

typedef struct WordNode *WordNodePtr;
typedef struct WordNode {
    Word word;
    WordNodePtr next;
} WordNode;

typedef struct labelList *labelListPtr;
typedef struct labelList{
    char labelName[LABEL_LENGTH];
    long address;
    Location codeOrData;
    LabelType entryOrExtern; // enum {entry, extern, none}
    labelListPtr next;
}labelList;


typedef struct externalList *externalListPtr;
typedef struct externalList{
    char labelName[LABEL_LENGTH];
    Location codeOrData;
    long address;
    externalListPtr next;
}externalList;


typedef struct entryList *entryListPtr;
typedef struct entryListList{
    char labelName[LABEL_LENGTH];
    Location codeOrData;
    long address;
    entryListPtr next;
}entryList;

typedef struct globalVariables{
    Bool errorFound;
    errorType type;
    int IC;
    int DC;
    int DCF;
    int ICF;
    int currentLine;
    char filename[FILE_NAME_LENGTH];
    FILE *file;
    labelListPtr headLabelTable;
    WordNodePtr headWordList;
    externalListPtr headExternList;
    entryListPtr  headEntryList;
} globalVariables;



#endif