//
//  helb.h
//  Hackers Evolution
//
//  Created by Eric Baur on 5/6/25.
//

// 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
//   ! " # $ % & ' ( ) *  +  ,  -  .  /

#ifndef HELC_H
#define HELC_H

#define PROG_SIZE 24
#define TAPE_SIZE 24

#define MAX_EXECUTION 10000

typedef enum Instruction {
    NOP,    //   - skips this spot
    RED,    // ! - inserts <value> characters to the stack from STDIN or DATA
    DUP,    // " - duplicates current tape value to new location(s) <value> steps away
            //
    INS,    // # - inserts <value> to the stack, moves the tape forward one
    OUT,    // $ - outputs the stack as a (null-terminated) string (maybe use <value> for bitmask options?
            // 0000 = defaults: outputs entire stack (null-terminated) as ASCII string
            // 1000 = output first value (default: null-terminated)
            // 0100 = output as a number (default: assume ASCII
            // 0010 = ???
            // 0001 = reverse string
    SWP,    // % - swaps 2 values on the stack: the top value and the value at top-<value>: 0 creates a no-op.
    AND,    // & - allows the <value> of this cell to bit-wise concat with the next <value>, using the instruction in the next code point for the next instruction. This allows for entering hexidecimal values directly, for example
    INC,    // ' - increment the position on the tape by <value> spots. If <value> is zero, increment the current stack value by 1 instead
    ANC,    // ( - start of looping construct/conditional, if stack is zero or less jumps just past matching END. <value> is currently ignored
    END,    // ) - jumps back to anchor, maybe? does the <value> matter? maybe a bitmask?
            // By default, jumps back to matching ANC if stack value is non-zero
            // If <value> is non-zero, consumes that number of stack values instead and jumps if there are values left.
    MUL,    // * - multiple current stack entry by the stack entry pointed to my <value>; consume top value and push answer to the stack at <value>
    ADD,    // + - adds previous stack entry to the one <value> steps before *that* and update current stack value with the answer.
    DEC,    // ' - decrement the position on the tape by <value> spots. If <value> is zero, decrement the current stack value by 1 instead
    SUB,    // - - reduces current <value> stack entry by top entry; consume top entry
    DAT,    // . - the next <value> code points should be interpreted as ASCII data values
    DIV     // / - divide current stack entry by <value> (with tuncation); replace stack entry
} Instruction;

typedef struct CodePoint {
    unsigned char inst: 4, val: 4;
} CodePoint;

typedef struct Program {
    int pos;
    CodePoint code[PROG_SIZE];
} Program;

typedef struct Tape {
    int pos;
    int values[TAPE_SIZE];
} Tape;

typedef struct Instance {
    Program prog;
    Tape tape;
} Instance;

char instructionToChar(Instruction inst);
Instruction charToInstruction(const char c);
int defaultForInstruction(Instruction inst);

int step(Program *prog, Tape *tape, int additionalValue);
void executeWithTape(Program *prog, Tape *tape);
void execute(Program *prog);
Tape* tapeFromExecution(Program *prog);
void printTape(Tape tape);
void printProg(Program *program);

Program* newProg(void);
Tape* newTape(void);
Instance* newInstance(void);
CodePoint codePointFromString(const char *str);
Program* progFromString(const char *str);

#endif
