cmake_minimum_required(VERSION 3.19)
project(RoniProject C)

set(CMAKE_C_STANDARD 90)

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

add_executable(RoniProject main.c main.h default.h firstPass.c firstPass.h inputAnalyzed.c inputAnalyzed.h EntryAndExtern.c WordList.c WordList.h lableList.c lableList.h instructionAnalyzed.c instructionAnalyzed.h directiveAnalyzed.c directiveAnalyzed.h secondPass.c secondPass.h printErrors.c printErrors.h EntryAndExtern.h )
