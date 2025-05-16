//
//  main.c
//  he
//
//  Created by Eric Baur on 5/13/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helc.h"

int main(int argc, const char * argv[]) {
    char program[] = "#2#4*0$0";
   
    CodePoint *code = progFromString(program);
    
    execute(code);
    
    free(code);
    
    return 0;
}
