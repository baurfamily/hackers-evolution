//
//  main.c
//  he-test
//
//  Created by Eric Baur on 5/20/25.
//

#include <stdio.h>

#include "tests.h"

int main(int argc, const char * argv[]) {
    test_canParse();
    test_canDoMath();

    return 0;
}
