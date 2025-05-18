//
//  instructions.c
//  Hackers Evolution
//
//  Created by Eric Baur on 5/15/25.
//

#include "helc.h"
#include "instructions.h"

#include <stdio.h>

void instNOP(char val, Program *prog, Stack *stack) {
    // this space intentionally left blank
}

void instRED(char val, Program *prog, Stack *stack);

//TODO: so much more to implement here...
void instDUP(char val, Program *prog, Stack *stack) {
    char dup = stack->values[stack->pos];
    stack->pos++;
    stack->values[stack->pos] = dup;
}

void instINS(char val, Program *prog, Stack *stack) {
    //TODO: this is super unsafe (can buffer overflow)
    if (stack->pos >= PROG_SIZE) {
        printf("!Stack overrun in INS.");
        return;
    }
    stack->pos++;
    stack->values[stack->pos] = val;
}

void instOUT(char val, Program *prog, Stack *stack) {
    // this has implicit (and destructive) casting
    printf("%s\n", stack->values);
    
    // need to account for options
}

void instSWP(char val, Program *prog, Stack *stack) {
    // save current position locally for convinience
    // must have at least 2 values for this to work
    if (stack->pos<val) {
        printf("!Stack underrun in SWP.");
        return;
    }
    
    if (val > 0) {
        char swp = stack->values[stack->pos];
        stack->values[stack->pos] = stack->values[stack->pos-val];
        stack->values[stack->pos-val] = swp;
    } else {
        stack->values[stack->pos] = 0;
        stack->pos--;
    }
}

void instAND(char val, Program *prog, Stack *stack);

void instINC(char val, Program *prog, Stack *stack) {
    // stack overrun implicitly inserts a value
    if (stack->pos<0) {
        stack->pos = 0;
        stack->values[0] = 0;
    }
    stack->values[stack->pos] = stack->values[stack->pos] + val;
};

void instANC(char val, Program *prog, Stack *stack) {
    // note: value ignored
    if (stack->values[stack->pos] <= 0) {
        
    }
}

void instEND(char val, Program *prog, Stack *stack) {
    // note: value ignored
}


void instMUL(char val, Program *prog, Stack *stack) {
    if (stack->pos<val) {
        printf("Stack underrun in MUL.");
        // for "safety" we reset to a simple case
        val = (stack->pos<1 ? 1 : 0);
    }
    
    // save off relevant values
    int first = stack->values[stack->pos];
    int second = (val>0 ? stack->values[stack->pos-val] : first);

    // shift values, if necessary, to cover the consumed value
    for (int i=1; i<val; i++) {
        stack->values[stack->pos-i] = stack->values[stack->pos-i];
    }
    
    if (val>0) {
        // save result
        stack->values[stack->pos-1] = second * first;
        
        // reduce stack
        stack->values[stack->pos] = 0;
        stack->pos--;
    } else {
        // if val points to top of stack, square it in place
        stack->values[stack->pos] = first * first;
    }
}

void instADD(char val, Program *prog, Stack *stack) {
    if (stack->pos<val) {
        printf("Stack underrun in MUL.");
        // for "safety" we reset to a simple case
        val = (stack->pos<1 ? 1 : 0);
    }
    
    // save off relevant values
    int first = stack->values[stack->pos];
    int second = (val>0 ? stack->values[stack->pos-val] : first);

    // shift values, if necessary, to cover the consumed value
    for (int i=1; i<val; i++) {
        stack->values[stack->pos-i] = stack->values[stack->pos-i];
    }
    
    if (val>0) {
        // save result
        stack->values[stack->pos-1] = second + first;
        
        // reduce stack
        stack->values[stack->pos] = 0;
        stack->pos--;
    } else {
        // if val points to top of stack, square it in place
        stack->values[stack->pos] = first + first;
    }
}

void instDEC(char val, Program *prog, Stack *stack) {
    // stack overrun implicitly inserts a value
    if (stack->pos<0) {
        stack->pos = 0;
        stack->values[0] = 0;
    }
    stack->values[stack->pos] = stack->values[stack->pos] - val;
}

void instSUB(char val, Program *prog, Stack *stack) {
    if (stack->pos<val) {
        printf("Stack underrun in MUL.");
        // for "safety" we reset to a simple case
        val = (stack->pos<1 ? 1 : 0);
    }
    
    // save off relevant values
    int first = stack->values[stack->pos];
    int second = (val>0 ? stack->values[stack->pos-val] : first);

    // shift values, if necessary, to cover the consumed value
    for (int i=1; i<val; i++) {
        stack->values[stack->pos-i] = stack->values[stack->pos-i];
    }
    
    if (val>0) {
        // save result
        stack->values[stack->pos-1] = second - first;
        
        // reduce stack
        stack->values[stack->pos] = 0;
        stack->pos--;
    } else {
        // if val points to top of stack, square it in place
        stack->values[stack->pos] = first - first;
    }
}

void instDAT(char val, Program *prog, Stack *stack);

void instDIV(char val, Program *prog, Stack *stack) {
    if (stack->pos<val) {
        printf("Stack underrun in MUL.");
        // for "safety" we reset to a simple case
        val = (stack->pos<1 ? 1 : 0);
    }
    
    // save off relevant values
    int first = stack->values[stack->pos];
    int second = (val>0 ? stack->values[stack->pos-val] : first);

    // shift values, if necessary, to cover the consumed value
    for (int i=1; i<val; i++) {
        stack->values[stack->pos-i] = stack->values[stack->pos-i];
    }
    
    if (val>0) {
        // save result
        stack->values[stack->pos-1] = second / first;
        
        // reduce stack
        stack->values[stack->pos] = 0;
        stack->pos--;
    } else {
        // if val points to top of stack, square it in place
        stack->values[stack->pos] = first / first;
    }
}

