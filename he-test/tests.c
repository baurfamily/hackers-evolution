//
//  tests.c
//  Hackers Evolution
//
//  Created by Eric Baur on 5/20/25.
//

// rolling my own for now

#include "tests.h"

void test_canParse(void) {
    Program *prog;
    
    // fully qualified
    prog = PARSE(" !\"#$%&'()*+,-./");
    ASSERT(prog->code[0].inst == NOP); ASSERT(prog->code[0].val == 0);
    //ASSERT(prog->code[1].inst == RED); //conflicts with color define
    ASSERT(prog->code[2].inst == DUP); ASSERT(prog->code[2].val == 1);
    ASSERT(prog->code[3].inst == INS); ASSERT(prog->code[3].val == 1);
    ASSERT(prog->code[4].inst == OUT); ASSERT(prog->code[4].val == 0);
    ASSERT(prog->code[5].inst == SWP); ASSERT(prog->code[5].val == 1);
    ASSERT(prog->code[6].inst == AND); ASSERT(prog->code[6].val == 10);
    ASSERT(prog->code[7].inst == INC); ASSERT(prog->code[7].val == 1);
    ASSERT(prog->code[8].inst == ANC); ASSERT(prog->code[8].val == 0);
    ASSERT(prog->code[9].inst == END); ASSERT(prog->code[9].val == 0);
    ASSERT(prog->code[10].inst == MUL); ASSERT(prog->code[10].val == 1);
    ASSERT(prog->code[11].inst == ADD); ASSERT(prog->code[11].val == 1);
    ASSERT(prog->code[12].inst == DEC); ASSERT(prog->code[12].val == 1);
    ASSERT(prog->code[13].inst == SUB); ASSERT(prog->code[13].val == 1);
    ASSERT(prog->code[14].inst == DAT); ASSERT(prog->code[14].val == 1);
    ASSERT(prog->code[15].inst == DIV); ASSERT(prog->code[15].val == 1);
    
    prog = PARSE("#1+1+1");
    
    ASSERT(prog->code[0].inst == INS);
    ASSERT(prog->code[0].val == 1);
    ASSERT(prog->code[1].inst == ADD);
    ASSERT(prog->code[1].val == 1);
}

void test_canLoop(void) {
    Tape *tape;
    
    // rewind tape
    tape = RUN("#2#1#1#1,(,)'");
    ASSERT_VALUE(2);
    
    // one way to generate multiple numbers via a loop
    tape = RUN("#4,('#1,3#1%-)'");
    ASSERT_VALUE_AT(0,0);
    ASSERT_VALUE_AT(1,1);
    ASSERT_VALUE_AT(2,1);
    ASSERT_VALUE_AT(3,1);
    ASSERT_VALUE_AT(4,1);
    ASSERT_VALUE_AT(5,0);
    
    // delete all values: (-0,)
    tape = RUN("#1#1#1#1#1#1#1,(-0,)");
    for (int i=0; i<PROG_SIZE; i++) {
        ASSERT_VALUE_AT(i,0);
    }
    
    // counting loop
    // #5      <-- counting variable
    // #1      <-- priming the loop
    // (
    //   (     <-- loop to move to end of tape
    //     '
    //   )
    //   ,     <-- shift back one
    //   "     <-- duplicate current value
    //   (2    <-- loop to move back to beginning, but offset the check
    //         <-- this makes it stop on the 1 that was primed
    //     ,
    //   )
    //   -     <-- subtract the first one from the counting var
    //   '     <-- shift forward one (into the empty space left from -
    //   #1    <-- re-insert our prime value
    // )2      <-- loop back!
    // ,       <-- shift back to insert a marker
    // #F
    // ,
    tape = RUN("#5#1(('),\"(2,)-'#1)2,#F,");
    ASSERT_VALUE_AT(0,15);
    ASSERT_VALUE_AT(1,1);
    ASSERT_VALUE_AT(2,1);
    ASSERT_VALUE_AT(3,1);
    ASSERT_VALUE_AT(4,1);
    ASSERT_VALUE_AT(5,1);
}

void test_canDoMath(void) {
    Tape *tape;
    
    // in a row
    tape = RUN("#1#2#3#4,+++");
    ASSERT_VALUE(10);
    
    // testing addition
    tape = RUN("#3#2,+1");
    ASSERT_VALUE(5);
    
    // testing addition (with implicit arguments)
    tape = RUN("#3#2,+");
    ASSERT_VALUE(5);

    // doubling
    tape = RUN("#3,+0");
    ASSERT_VALUE(6);
    
    // adding at distance
    tape = RUN("#1#2#3,+2");
    ASSERT_VALUE(4);
    
    //subtraction
    tape = RUN("#3#2,-");
    ASSERT_VALUE(1)
    
    //removal (additive identity)
    tape = RUN("#3,-0");
    ASSERT_VALUE(0)
    
    //multiplication
    tape = RUN("#2#3,*");
    ASSERT_VALUE(6);
    
    // squaring
    tape = RUN("#3,*0");
    ASSERT_VALUE(9);
    
    // dividing
    tape = RUN("#A#2,/");
    ASSERT_VALUE(5);
    
    // division truncates
    tape = RUN("#A#3,/");
    ASSERT_VALUE(3);
    
    // multiplicitive identity
    tape = RUN("#3,/0");
    ASSERT_VALUE(1);
    
}

