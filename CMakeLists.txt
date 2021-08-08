cmake_minimum_required(VERSION 3.19)
project(RoniProject C)

set(CMAKE_C_STANDARD 90)

add_executable(RoniProject main.c main.h default.h firstPass.c firstPass.h inputAnalyzed.c inputAnalyzed.h EntryAndExtern.c WordList.c WordList.h lableList.c lableList.h instructionAnalyzed.c instructionAnalyzed.h directiveAnalyzed.c directiveAnalyzed.h secondPass.c secondPass.h)