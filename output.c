
void outputObject(globalVariables *vars) {
    int i = 0;

    // first line
    // vars->IC

    //vars->DC
    // iterate over labelListPtr for labels and headWordList for memory data
    labelListPtr headLabelTable = &vars->headLabelTable;
    WordNodePtr headWordList = &vars->headWordList;

    while (headLabelTable) {
        printf("%ld", headLabelTable->address); // print label address
        for (i = 0; i < 4; ++i) {
            if (!headWordList) {
                break;
            }
            printf("%ld ", headLabelTable->address); // print label address
            if (headWordList->word.wordType == Instruction) {
                printf("%x ", headWordList->word.instruction);
            } else {
                if (headWordList->word.wordType == Directive) {
                    printf("%x ", headWordList->word.directive);
                }
                headWordList = headWordList->next;
            }
            headLabelTable = headLabelTable->next;
        }
    }

    void outputEntries(globalVariables *vars) {

    }
    }

    void outputExternals(globalVariables *vars) {

    }


    void createOutput(globalVariables *vars) {
        outputObject(vars);
        outputEntries(vars);
        outputExternals(vars);
    }
