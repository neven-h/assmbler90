main:main.o firsPass.o secondPass.o inputAnalyzed.o instructionAnalyzed.o directiveAnalyzed.o EntryAndExtern.o lableList.o WordList.o

main.o: main.c
	gcc -c -g main.c -ansi -Wall -pedantic -o main.o

firstPass.o: firstPass.c
	gcc -c -g main.c -ansi -Wall -pedantic -o firstPass.o

secondPass.o: secondPass.c
	gcc -c -g main.c -ansi -Wall -pedantic -o secondPass.o

inputAnalyzed.o: inputAnalyzed.c
	gcc -c -g main.c -ansi -Wall -pedantic -o inputAnalyzed.o


instructionAnalyzed.o:instructionAnalyzed.c
	gcc -c -g main.c -ansi -Wall -pedantic -o instructionAnalyzed.o


directiveAnalyzed.o:directiveAnalyzed.c
	gcc -c -g main.c -ansi -Wall -pedantic -o directiveAnalyzed.o


EntryAndExtern.o:EntryAndExtern.c
	gcc -c -g main.c -ansi -Wall -pedantic -o EntryAndExtern.o


lableList.o:lableList.c
	gcc -c -g main.c -ansi -Wall -pedantic -o lableList.o



WordList.o:WordList.c
	gcc -c -g main.c -ansi -Wall -pedantic -o WordList.o

