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
#define RED printf("\n\033[0;31m");
#define GREEN printf("\n\033[0;32m");
#define YELLOW printf("\n\033[0;33m");
#define BLUE printf("\n\033[0;34m");
#define PURPLE printf("\n\033[0;35m");
#define CYAN printf("\n\033[0;36m");
#define WHITE printf("\n\033[0;37m");

#define PASS GREEN printf(" pass "); RESET
#define FAIL RED printf(" fail "); RESET

#define PARSE(text) progFromString(text);
#define RUN(text) tapeFromExecution(progFromString(text), NULL);
#define RUN_MORE(text) tapeFromExecution(progFromString(text), tape);
#define ASSERT(test) if (test) { PASS; } else { FAIL; }; printf(#test);
#define ASSERT_VALUE(val) if (tape->values[tape->pos] == val) { PASS; } else { FAIL; }; printf("%d == %d", tape->values[tape->pos], val);
#define ASSERT_VALUE_AT(index,val) if (tape->values[(tape->pos+index)%TAPE_SIZE] == val) { PASS; } else { FAIL; }; printf("%d == %d", tape->values[(tape->pos+index)%TAPE_SIZE], val);


void test_canParse(void);
void test_canLoop(void);
void test_canDoMath(void);
void test_tapeManipulation(void);
void test_tapeManipulation_swap(void);
