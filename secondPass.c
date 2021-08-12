//
// Created by ronia on 08/08/2021.
//

#include "secondPass.h"
#include "default.h"
#include "lableList.h"
#include "WordList.h"
#include "inputAnalyzed.h"
#include "instructionAnalyzed.h"
#include "directiveAnalyzed.h"


void firstPass(globalVariables *vars) {

    int lineAnalyzed;
    Bool hasLabel,EntryLabel;
    WordType word;
    Bool directiveSecondPass;
    Bool instructionFirstPass;

    char line[LINE_LENGTH] = {0};
    char before[LINE_LENGTH] = {0};
    char after[LINE_LENGTH] = {0};
    char lineCpyAfterLabel[LINE_LENGTH] = {0};

    char fileName[FILE_NAME_LENGTH + AS_EXTENSION_LENGTH];
    strcpy(vars->filename, fileName);

    while (!feof(vars->file)) {


        memset(line, 0, LINE_LENGTH);
        memset(before, 0, LINE_LENGTH);
        memset(after, 0, LINE_LENGTH);
        memset(lineCpyAfterLabel, 0, LINE_LENGTH);

        fgets(line, LINE_LENGTH, stdin);

        strip(line); /*strip white chars*/
        lineAnalyzed = isEmptyOrCommandLine(line);
        if (lineAnalyzed == 1) continue; /*if the line is an empty line or command line - the assembler ignores*/

        /*create a new label node*/
        labelListPtr currentLabel = (labelListPtr) calloc(1, sizeof(labelListPtr));
        if (!currentLabel) {
            exit(0);
        }

        /*check if we have label - if we do - skip this label*/
        /*add bool function to check if we have a label*/
        hasLabel = foundLabel(line, before, after, vars, currentLabel);

        if (hasLabel == True) { /*we found a label*/
            strcpy(lineCpyAfterLabel, after);
        } else { /*we couldn't fina d label, by split fun before=line*/
            strcpy(lineCpyAfterLabel, line);
        }

        strip(lineCpyAfterLabel);
        word = directiveOrInstruction(lineCpyAfterLabel, before, after, vars); /*check if Directive or Instruction or none*/
        if (word == Directive) {
            directiveSecondPass = isDirectiveSecondPass(before, after, vars, hasLabel, currentLabel);
            if (directiveSecondPass == False)
                continue; /* not an ENTRY directive - continue to the next line*/
            if(directiveSecondPass ==True) /*it's an Entry Directive*/
            {
                EntryLabel=isLabelEntry(&(vars->headLabelTable),after,vars); /*check if the given entry label exists and add to the label list the attribute to this label as -entry*/
                if(EntryLabel==True || EntryLabel==False)
                    continue;
            }
        }
        if(word == Instruction)
        {

        }

    }
}

Bool isDirectiveSecondPass(char *before,char after ,globalVariables *vars, Bool hasLabel, labelListPtr currentLabel) {
    int directiveNum;
    directiveNum = isValidDirectiveName(before); /*find if it's a valid directive and the num*/
    if (directiveNum != DIRECTIVE_ENTRY)
    {
            return False; /*if it's not an entry continue to the next line*/
    }
    return True;

}

