//
//  helb.h
//  Hackers Evolution
//
//  Created by Eric Baur on 5/6/25.
//

// 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
//   ! " # $ % & ' ( ) *  +  ,  -  .  /

#define PROG_SIZE 1023
#define STACK_SIZE 1023

typedef enum Instruction {
    NOP,    //   - skips this spot, (competative: <value> must be 0, otherwise terminate)
    RED,    // ! - inserts <value> characters to the stack from STDIN or DATA
    DUP,    // " - duplicates / copies - need to figure out symantics here - bitmask?
            // 0000 = duplicates top stack value back onto the stack
            // 1000 = interprets next stack value as the number of positions to copy, starting just after it
            // 0100 = interprets next stack value as a relative position to copy to
            // 0010 = interprest next stack value as a relative position to copy from (instead of stack)
            // 0001 = reverse the order during the copy
    INS,    // # - inserts <value> to the stack
    OUT,    // $ - outputs the stack as a (null-terminated) string (maybe use <value> for bitmask options?
            // 0000 = defaults: outputs entire stack (null-terminated) as ASCII string
            // 1000 = output first value (default: null-terminated)
            // 0100 = output as a number (default: assume ASCII
            // 0010 =
            // 0001 = reverse string
    SWP,    // % - swaps 2 values on the stack: the top value and the value at top-<value>: 0 drops top value (swap with nothing?)
    AND,    // & - allows the <value> of this cell to bit-wise concat with the next <value>, using the instruction in the next code point for the next instruction
    INC,    // ' - increment current value on the stack by <value>, replace stack entry
    ANC,    // ( - start of looping construct/conditional, if stack is zero or less jumps just past matching END
    END,    // ) - jumps back to anchor, maybe? does the <value> matter? maybe a bitmask?
    MUL,    // * - multiple current stack entry by the stack entry pointed to my <value>; consume both values and push answer to the stack
    ADD,    // + - add current stack entry to second stack entry; consume both values and push answer to the stack
    DEC,    // ' - decrement the current value on the stack by <value>; replace stack entry
    SUB,    // - - reduces current stack entry by <value; replace stack entry
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

typedef struct Stack {
    int pos;
    int values[STACK_SIZE];
} Stack;

typedef struct Instance {
    Program prog;
    Stack stack;
} Instance;

char instructionToChar(Instruction inst);
Instruction charToInstruction(const char c);

int step(CodePoint code, Stack *stack);
void execute(CodePoint *code);
void printStack(Stack stack);
void printProg(CodePoint *program);

CodePoint* newProg(void);
Stack* newStack(void);
Instance* newInstance(void);
CodePoint codePointFromString(const char *str);
CodePoint* progFromString(const char *str);
