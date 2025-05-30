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

void instNOP(int val, Program *prog, Tape *tape) {
    // this space intentionally left blank
}

void instRED(int val, Program *prog, Tape *tape) {
    int datVal = findDAT(prog);
    
    // dataPos may have been modiifed by previous call
    int start = prog->dataPos;
    
    // if val was zero, use val from data element
    // which could also be 0  - or not found (and be 0)
    int end = prog->dataPos + (val == 0 ? datVal : val);
    
    for (int i=start; i<end; i++) {
        CodePoint cp = prog->code[i];
        // reconstruct the original value from the broken up bits
        // might be able to do a simple cast of the struct
        CURRENT_VALUE = (cp.inst << 4) | cp.val;
        MOVE_TAPE(1);
    }
    prog->dataPos = end % PROG_SIZE;
}

// return the val of the data element we found
// OR, if it was zero, find the length of the data
// OR, if nothing else, return 0
int findDAT(Program *prog) {
    for (int i=0; i<PROG_SIZE; i++) {
        int consideredPos = (prog->dataPos + i) % PROG_SIZE;
        CodePoint cp = prog->code[consideredPos];
        
        if (cp.inst == DAT) {
            // set the new data position
            prog->dataPos = (consideredPos + 1) % PROG_SIZE;
            if (cp.val == 0) {
                // look for end of data
                for (int j=0; j<PROG_SIZE; j++) {
                    int finalConsideredPos = (consideredPos + j) % PROG_SIZE;
                    
                    CodePoint cpEnd = prog->code[finalConsideredPos];
                    if (cpEnd.inst == 0 && cpEnd.val == 0) {
                        
                        
                        // return the number of data elements available to read
                        return finalConsideredPos - consideredPos - 1;
                    }
                }
            } else {
                // set our current data position
                prog->dataPos = (consideredPos+1) % PROG_SIZE;
                
                // just return the data value
                return cp.val;
            }
        }
    }
    // okay, not sure this makes sense, but if we don't find a DAT mark
    // we'll just keep reading from where we left off (which might have
    // been the start of the program if there are no DAT instructions
    return 0;
}

//TODO: so much more to implement here...
void instDUP(int val, Program *prog, Tape *tape) {
    // special case for 0: duplicate *into* this spot from previous value
    if (val == 0) {
        CURRENT_VALUE = PREVIOUS_VALUE;
    } else {
        for (int i=1; i<=val; i++) {
            VALUE_AT(i) = CURRENT_VALUE;
        }
    }
}

void instINS(int val, Program *prog, Tape *tape) {
    CURRENT_VALUE = val;
    MOVE_TAPE(1);
}

// this seems inefficient...
void instOUT(int val, Program *prog, Tape *tape) {
    int end = val == 0 ? PROG_SIZE : val;
    
    for (int i=0; i<end; i++) {
        if (CURRENT_VALUE == 0) break;
        printf("%c", CURRENT_VALUE);
        MOVE_TAPE(1);
    }
}

void instSWP(int val, Program *prog, Tape *tape) {
    // experiment with 0 swapping left-right values
    // but leaving current tape position alone
    if (val==0) {
        char swp = PREVIOUS_VALUE_AT(1);
        PREVIOUS_VALUE_AT(1) = PREVIOUS_VALUE_AT(-1);
        PREVIOUS_VALUE_AT(-1) = swp;
    } else {
        char swp = CURRENT_VALUE;
        CURRENT_VALUE = PREVIOUS_VALUE_AT(val);
        PREVIOUS_VALUE_AT(val) = swp;
    }
}

void instAND(int val, Program *prog, Tape *tape) {
    MOVE_PROG(1);
    step(prog, tape, val);
}

void instINC(int val, Program *prog, Tape *tape) {
    // experiment with special meaning for 0
    if (val == 0) {
        CURRENT_VALUE++;
    } else {
        MOVE_TAPE(val);
    }
};

void instANC(int val, Program *prog, Tape *tape) {
    if (PREVIOUS_VALUE_AT(val) <= 0) {
        findEND(prog);
    }
}

void findEND(Program *prog) {
    int anchorCount = 1;
    bool endFound = false;
    while (!endFound && prog->pos < PROG_SIZE-1) {
        MOVE_PROG(1);
        switch (CURRENT_PROG.inst) {
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
    if (PREVIOUS_VALUE_AT(val) <= 0 ) {
        return;
    }
    findANC(prog);
}

// weird stuff will happen if the () aren't matched
void findANC(Program *prog) {
    int anchorCount = 1;
    
    for (int i=1; i<=PROG_SIZE; i++) {
        MOVE_PROG(-1);
        switch (CURRENT_PROG.inst) {
            case END:
                anchorCount++;
                break;
            case ANC:
                anchorCount--;
                if (anchorCount <= 0) {
                    // shift one more so the ANC executes next
                    // weirdly, it's possible for the loop req.
                    // to not match between ANC and END
                    MOVE_PROG(-1);
                    return;
                }
                break;
        }
    }
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
    if (val == 0) {
        CURRENT_VALUE--;
    } else {
        MOVE_TAPE(-val);
    }
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

void instDAT(int val, Program *prog, Tape *tape)  {
    // this marks data, the <value> is the number of
    // prog steps that are considered data
    // this is NOT validated on read, only used to skip
    // what we *assume* are not valid program instructions
    // val of 0 means "skip until a zero"
    if (val == 0) {
        for (int i=0; i<PROG_SIZE; i++) {
            MOVE_PROG(1);
            if (CURRENT_PROG.inst == 0 && CURRENT_PROG.val == 0) {
                break;
            }
        }
    } else {
        MOVE_PROG(val);
    }
}

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

