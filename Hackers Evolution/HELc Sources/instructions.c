//
//  instructions.c
//  Hackers Evolution
//
//  Created by Eric Baur on 5/15/25.
//

#include "helc.h"
#include "instructions.h"

#include <stdio.h>
#include <stdbool.h>

void instNOP(int val, Program *prog, Stack *stack) {
    // this space intentionally left blank
}

void instRED(int val, Program *prog, Stack *stack);

//TODO: so much more to implement here...
void instDUP(int val, Program *prog, Stack *stack) {
    // value is ignored for now, not sure exaclty how I want this to work
    char dup = stack->values[stack->pos];
    stack->pos++;
    stack->values[stack->pos] = dup;
}

void instINS(int val, Program *prog, Stack *stack) {
    //TODO: this is super unsafe (can buffer overflow)
    if (stack->pos >= PROG_SIZE) {
        printf("!Stack overrun in INS.");
        return;
    }
    stack->pos++;
    stack->values[stack->pos] = val;
}

// this seems inefficient...
void instOUT(int val, Program *prog, Stack *stack) {
    bool oneVal   = ( val & 1 );
    bool decimal  = ( val & 2 );
//    bool notImp   = ( val & 4 );
    bool reversed = ( val & 8 );
    
    if (oneVal) {
        int pos = (reversed ? stack->pos : 0);
        
        if (decimal) {
            printf("%d", stack->values[pos]);
        } else {
            printf("%c", (char)stack->values[pos]);
        }
    } else {
        if (reversed) {
            for (int i=stack->pos; i>=0; i--) {
                if (decimal) {
                    printf("%d ", stack->values[i]);
                } else {
                    printf("%c", (char)stack->values[i]);
                }
            }
        } else {
            for (int i=0; i<=stack->pos; i++) {
                if (decimal) {
                    printf("%d ", stack->values[i]);
                } else {
                    printf("%c", (char)stack->values[i]);
                }
            }
        }
    }
    
    // not sure if I really want this
    printf("\n");
}

void instSWP(int val, Program *prog, Stack *stack) {
    // if we don't have enough values to make this work...
    // just silently exit
    if (stack->pos<val) { return; }
    
    if (val > 0) {
        char swp = stack->values[stack->pos];
        stack->values[stack->pos] = stack->values[stack->pos-val];
        stack->values[stack->pos-val] = swp;
    } else {
        stack->values[stack->pos] = 0;
        stack->pos--;
    }
}

void instAND(int val, Program *prog, Stack *stack) {
    step(prog, stack, val);
}

void instINC(int val, Program *prog, Stack *stack) {
    // stack overrun implicitly inserts a value
    if (stack->pos<0) {
        stack->pos = 0;
        stack->values[0] = 0;
    }
    stack->values[stack->pos] = stack->values[stack->pos] + val;
};

void instANC(int val, Program *prog, Stack *stack) {
//    printf("compare %d to %d", val, stack->values[stack->pos]);
    // stack index out of bounds ends loop
    if (stack->pos < val || stack->values[stack->pos-val] <= 0) {
        int anchorCount = 1;
        bool endFound = false;
        while (!endFound && prog->pos < PROG_SIZE) {
            prog->pos = prog->pos + 1;
            switch (prog->code[prog->pos].inst) {
                case ANC:
                    anchorCount++;
                case END:
                    anchorCount--;
                    if (anchorCount <= 0)
                        endFound = true;
            }
        }
    }
}

void instEND(int val, Program *prog, Stack *stack) {
    // note: value currently ignored
    int anchorCount = 1;
    bool endFound = false;
    while (!endFound && prog->pos >= 0) {
        prog->pos = prog->pos - 1;
        switch (prog->code[prog->pos].inst) {
            case END:
                anchorCount++;
            case ANC:
                anchorCount--;
                if (anchorCount <= 0)
                    endFound = true;
        }
    }
    // decrement one more so the ANC executes next
    prog->pos = prog->pos - 1;
}


void instMUL(int val, Program *prog, Stack *stack) {
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

void instADD(int val, Program *prog, Stack *stack) {
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

void instDEC(int val, Program *prog, Stack *stack) {
    // stack overrun implicitly inserts a value
    if (stack->pos<0) {
        stack->pos = 0;
        stack->values[0] = 0;
    }
    stack->values[stack->pos] = stack->values[stack->pos] - val;
}

void instSUB(int val, Program *prog, Stack *stack) {
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

void instDAT(int val, Program *prog, Stack *stack);

void instDIV(int val, Program *prog, Stack *stack) {
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

