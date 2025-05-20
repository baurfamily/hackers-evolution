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

#define TAPE_DELTA(val) ((TAPE_SIZE + tape->pos + val) % TAPE_SIZE)
#define MOVE_TAPE(val) tape->pos = TAPE_DELTA(val)
#define CURRENT_VALUE (tape->values[tape->pos])
#define PREVIOUS_VALUE (tape->values[TAPE_DELTA(-1)])
// implicitly subtracts
#define PREVIOUS_VALUE_AT(val) (tape->values[TAPE_DELTA(-(val))])

void instNOP(int val, Program *prog, Tape *tape) {
    // this space intentionally left blank
}

void instRED(int val, Program *prog, Tape *tape);

//TODO: so much more to implement here...
void instDUP(int val, Program *prog, Tape *tape) {
    tape->values[TAPE_DELTA(val)] = tape->values[tape->pos];;
}

void instINS(int val, Program *prog, Tape *tape) {
    tape->values[tape->pos] = val;
    MOVE_TAPE(1);
}

// this seems inefficient...
void instOUT(int val, Program *prog, Tape *tape) {
    bool oneVal   = ( val & 1 );
    bool decimal  = ( val & 2 );
//    bool notImp   = ( val & 4 );
    bool reversed = ( val & 8 );
    
    if (oneVal) {
        int pos = (reversed ? tape->pos : 0);
        
        if (decimal) {
            printf("%d", tape->values[pos]);
        } else {
            printf("%c", (char)tape->values[pos]);
        }
    } else {
        if (reversed) {
            for (int i=tape->pos; i>=0; i--) {
                if (decimal) {
                    printf("%d ", tape->values[i]);
                } else {
                    printf("%c", (char)tape->values[i]);
                }
            }
        } else {
            for (int i=0; i<=tape->pos; i++) {
                if (decimal) {
                    printf("%d ", tape->values[i]);
                } else {
                    printf("%c", (char)tape->values[i]);
                }
            }
        }
    }
    
    // not sure if I really want this
    printf("\n");
}

void instSWP(int val, Program *prog, Tape *tape) {
    // if we don't have enough values to make this work...
    // just silently exit, making 0 a no-op
    if (val==0) { return; }
    
    char swp = CURRENT_VALUE;
    CURRENT_VALUE = PREVIOUS_VALUE_AT(val);
    PREVIOUS_VALUE_AT(val) = swp;
}

void instAND(int val, Program *prog, Tape *tape) {
    step(prog, tape, val);
}

void instINC(int val, Program *prog, Tape *tape) {
    MOVE_TAPE(val);
};

void instANC(int val, Program *prog, Tape *tape) {
    if (tape->pos<(val-1)) {
        findEND(prog);
    }
}

void findEND(Program *prog) {
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

void instEND(int val, Program *prog, Tape *tape) {
    // if current tape is 0, there is nothing to do!
    if (CURRENT_VALUE <= 0 ) {
        return;
    }    
    findANC(prog);
}

void findANC(Program *prog) {
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

void instMUL(int val, Program *prog, Tape *tape) {
    // not sure how this would happen, but we don't want it
    if (val < 0) { return; }
    
    // save off relevant values
    int first = CURRENT_VALUE;
    int second = PREVIOUS_VALUE_AT(val);
    
    if (val > 0) {
        for (int i=val; i>0; i--) {
            PREVIOUS_VALUE_AT(i) = PREVIOUS_VALUE_AT(i-1);
        }
        
        // clear the value where we started
        CURRENT_VALUE = 0;
        
        // shift tape head
        MOVE_TAPE(-1);
    }
    // save result
    CURRENT_VALUE = second * first;
}

void instADD(int val, Program *prog, Tape *tape) {
    // not sure how this would happen, but we don't want it
    if (val < 0) { return; }
    
    // save off relevant values
    int first = CURRENT_VALUE;
    int second = PREVIOUS_VALUE_AT(val);
    
    if (val > 0) {
        for (int i=val; i>0; i--) {
            PREVIOUS_VALUE_AT(i) = PREVIOUS_VALUE_AT(i-1);
        }
        
        // clear the value where we started
        CURRENT_VALUE = 0;
        
        // shift tape head
        MOVE_TAPE(-1);
    }
    // save result
    CURRENT_VALUE = second + first;
}

void instDEC(int val, Program *prog, Tape *tape) {
    MOVE_TAPE(-val);
}

void instSUB(int val, Program *prog, Tape *tape) {
    // not sure how this would happen, but we don't want it
    if (val < 0) { return; }
    
    // save off relevant values
    int first = CURRENT_VALUE;
    int second = PREVIOUS_VALUE_AT(val);
    
    if (val > 0) {
        // shift numbers around on the tape
        for (int i=val; i>0; i--) {
            PREVIOUS_VALUE_AT(i) = PREVIOUS_VALUE_AT(i-1);
        }
        
        // clear the value where we started
        CURRENT_VALUE = 0;
        
        // shift tape head
        MOVE_TAPE(-1);
    }
    // save result
    CURRENT_VALUE = second - first;
}

void instDAT(int val, Program *prog, Tape *tape);

void instDIV(int val, Program *prog, Tape *tape) {
    // not sure how this would happen, but we don't want it
    if (val < 0) { return; }
    
    // save off relevant values
    int first = CURRENT_VALUE;
    int second = PREVIOUS_VALUE_AT(val);
    
    if (val > 0) {
        for (int i=val; i>0; i--) {
            PREVIOUS_VALUE_AT(i) = PREVIOUS_VALUE_AT(i-1);
        }
        
        // clear the value where we started
        CURRENT_VALUE = 0;
        
        // shift tape head
        MOVE_TAPE(-1);
    }
    // save result
    CURRENT_VALUE = second / first;
}

