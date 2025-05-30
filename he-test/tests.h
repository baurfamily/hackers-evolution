//
//  tests.h
//  Hackers Evolution
//
//  Created by Eric Baur on 5/20/25.
//

#include <stdio.h>

#include "helc.h"

#define RESET printf("\033[0m");

#define BLACK printf("\n\033[0;30m");
#define REDD printf("\n\033[0;31m");
#define GREEN printf("\n\033[0;32m");
#define YELLOW printf("\n\033[0;33m");
#define BLUE printf("\n\033[0;34m");
#define PURPLE printf("\n\033[0;35m");
#define CYAN printf("\n\033[0;36m");
#define WHITE printf("\n\033[0;37m");

#define PASS GREEN printf(" pass "); RESET
#define FAIL REDD printf(" fail "); RESET

#define SETUP Tape *tape; Program *prog;

#define PARSE(text) progFromString(text);
#define RUN(text) prog = progFromString(text); tape = tapeFromExecution(prog, NULL);
#define RUN_MORE(text) extendProg(prog, text); tape = tapeFromExecution(prog, tape);
#define ASSERT(test) if (test) { PASS; } else { FAIL; }; printf(#test);
#define ASSERT_VALUE(val) if (tape->values[prog->tapePos] == val) { PASS; } else { FAIL; }; printf("%d == %d", tape->values[prog->tapePos], val);
#define ASSERT_VALUE_AT(index,val) if (tape->values[(prog->tapePos+index)%TAPE_SIZE] == val) { PASS; } else { FAIL; }; printf("%d == %d", tape->values[(prog->tapePos+index)%TAPE_SIZE], val);


void test_canParse(void);
void test_canLoop(void);
void test_canDoMath(void);
void test_tapeManipulation_moving(void);
void test_tapeManipulation_swap(void);
void test_readingData(void);
void test_dataAndReadEdgeCases(void);
