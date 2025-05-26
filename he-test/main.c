//
//  main.c
//  he-test
//
//  Created by Eric Baur on 5/20/25.
//

#include <stdio.h>
#include <stdbool.h>

#include "tests.h"

bool isVerbose(void);
bool isQuiet(void);

int main(int argc, const char * argv[]) {
    test_canParse();
    test_canDoMath();
    test_canLoop();
    test_tapeManipulation_moving();
    test_tapeManipulation_swap();
    test_readingData();
    test_dataAndReadEdgeCases();
    
    return 0;
}

bool isVerbose(void) {
    return true;
}

bool isQuiet(void) {
    return false;
}
