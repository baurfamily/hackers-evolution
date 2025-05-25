//
//  helc.c
//  Hackers Evolution
//
//  Created by Eric Baur on 5/11/25.
//

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "helc.h"
#include "instructions.h"

int step(Program *prog, Tape *tape, int additionalVal) {

    CodePoint code = CURRENT_PROG;
    Instruction inst = code.inst;
    unsigned int val = code.val + (additionalVal << 4);
    
    if (inst==0) return 0;
    
    if (isVerbose()) {
        printf("\n%02d: %c%02d => ", prog->pos, instructionToChar(code.inst), val);
    }
    
    switch (inst) {
        case NOP: instNOP(val, prog, tape); break;
        case RED: instRED(val, prog, tape); break;;
        case DUP: instDUP(val, prog, tape); break;
        case INS: instINS(val, prog, tape); break;
        case OUT: instOUT(val, prog, tape); break;
        case SWP: instSWP(val, prog, tape); break;
        case AND: instAND(val, prog, tape); break;
        case INC: instINC(val, prog, tape); break;
        case ANC: instANC(val, prog, tape); break;
        case END: instEND(val, prog, tape); break;
        case MUL: instMUL(val, prog, tape); break;
        case ADD: instADD(val, prog, tape); break;
        case DEC: instDEC(val, prog, tape); break;
        case SUB: instSUB(val, prog, tape); break;
        case DAT: instDAT(val, prog, tape); break;;
        case DIV: instDIV(val, prog, tape); break;
        default:  printf("unmatched instruction\n");
    }
    MOVE_PROG(1);
    
    if (!isQuiet()) printTape(*tape);
    
    if (prog->pos == 0) {
        return -1;
    }
    return inst;
}

void executeWithTape(Program *prog, Tape *tape) {
    if (isVerbose()) {
        printf("\n");
        printProg(prog);
        printf("\n");
    }
    
    for (int i=0; i<MAX_EXECUTION; i++) {
        int returnCode = step(prog, tape, 0);
        if (returnCode==-1) break;
    }
}

void execute(Program *prog) {
    Tape tape = { .values={}, .pos=0 };
    executeWithTape(prog, &tape);
}

Tape* tapeFromExecution(Program *prog, Tape *tape) {
    if (tape == NULL) {
        tape = newTape();
    }
    executeWithTape(prog, tape);
    
    return tape;
}

Instruction charToInstruction(const char c) {
    switch (c) {
        case ' ': return NOP;
        case '!': return RED;
        case '"': return DUP;
        case '#': return INS;
        case '$': return OUT;
        case '%': return SWP;
        case '&': return AND;
        case '\'': return INC;
        case '(': return ANC;
        case ')': return END;
        case '*': return MUL;
        case '+': return ADD;
        case ',': return DEC;
        case '-': return SUB;
        case '.': return DAT;
        case '/': return DIV;
        default:  return NOP;
    }
}

int defaultForInstruction(Instruction inst) {
    switch (inst) {
        case NOP: return 0;
        case RED: return 0;
        case DUP: return 1;
        case INS: return 1;
        case OUT: return 0;
        case SWP: return 1;
        case AND: return 10; // no idea on this one
        case INC: return 1;
        case ANC: return 0;
        case END: return 0;
        case MUL: return 1;
        case ADD: return 1;
        case DEC: return 1;
        case SUB: return 1;
        case DAT: return 0;
        case DIV: return 1;
        default:  return 0;
    }
}

char instructionToChar(Instruction inst) {
    switch (inst) {
        case NOP: return ' ';
        case RED: return '!';
        case DUP: return '"';
        case INS: return '#';
        case OUT: return '$';
        case SWP: return '%';
        case AND: return '&';
        case INC: return '\'';
        case ANC: return '(';
        case END: return ')';
        case MUL: return '*';
        case ADD: return '+';
        case DEC: return ',';
        case SUB: return '-';
        case DAT: return '.';
        case DIV: return '/';
        default:  return '\0';
    }
}

Program* newProg(void) {
    Program* prog = (Program *)malloc(sizeof(Program));
    prog->pos = 0;
    
    // initialize array to null
    for (int i=0; i<PROG_SIZE; i++) {
        prog->code[i] = (CodePoint){ .inst=0, .val=0 };
    }
    
    return prog;
}

Tape* newTape(void) {
    Tape *tape = (Tape *)malloc(sizeof(Tape));
    tape->pos = 0;
    
    for (int i=0; i<PROG_SIZE; i++) {
        tape->values[i] = 0;
    }
    
    return tape;
}

Instance* newInstance(void) {
    Instance *instance = (Instance *)malloc(sizeof(Instance));
    
    // init everything
    for (int i=0; i<PROG_SIZE; i++) {
        instance->prog.code[i] = (CodePoint){ .inst=0, .val=0 };
        instance->prog.pos = 0;
        instance->tape.values[i] = 0;
        instance->tape.pos = 0;
    }
    
    return instance;
}

void printTape(Tape tape) {
    printf("\n[... ");
    for (int i=tape.pos-10; i<=tape.pos+10; i++) {
        int index = ((i > 0 ? i : TAPE_SIZE+i) % TAPE_SIZE);
        if (i==tape.pos) {
            if (tape.values[index] == 0) {
                printf("<.> ");
            } else {
                printf("<%d> ", tape.values[index]);
            }
        } else {
            if (tape.values[index] == 0) {
                printf(". ");
            } else {
                printf("%d ", tape.values[index]);
            }
        }
    }
    printf("...]");
}

void printProg(Program *prog) {
    char str[PROG_SIZE*2 + 1];
    char val[PROG_SIZE*2 + 1];
    
    for (int i=0; i<PROG_SIZE; i++) {
        Instruction inst = prog->code[i].inst;
        char c = instructionToChar(inst);
        str[i] = c;
        val[i] = (int)prog->code[i].val;
//        if (inst == 0)
//            break;
    }
    
    for (int i=0; i<PROG_SIZE; i++) {
        printf("%c%d ", str[i], val[i]);
    
        if ((i+1)%8 == 0) printf("\n");
        
//        if (prog->code[i].inst == 0) break;
    }
}

// this may not be a "readable" character
// this one is for the encoded version
CodePoint codePointFromEncodedChar(const char c) {
    int inst = c >> 4;
    int val = c & 15;
    return (CodePoint) { .inst = inst, .val = val };
}

CodePoint codePointFromString(const char *str) {
    return (CodePoint) {
        .inst = charToInstruction(str[0]),
        .val = (unsigned int)strtol(&str[1], NULL, 16)
    };
}

Program* progFromBytes(const char *str) {
    Program *prog = newProg();
    
    unsigned long length = strlen(str);

    int i;
    for (i=0; i<length; i++) {
        CodePoint cp = codePointFromEncodedChar(str[i]);
        prog->code[i] = cp;
        printf("(%d): inst: %c val: %d\n", i, instructionToChar(cp.inst), cp.val);
        
    }
    i++;
    prog->code[i] = (CodePoint){ .inst=0, .val=0 };
    
    return prog;
}

Program* progFromString(const char *str) {
    Program *prog = newProg();
    
    unsigned long length = strlen(str);
    
    int skipData = 0;
    bool skipToNop = false;

    int j=0;
    bool comment = false;
    for (int i=0; i<length; i++) {
        if (str[i] == ':') {
            comment = true;
        } else if (str[i] == ';') {
            comment = false;
        } else {
            if (!comment) {
                if (skipData > 0 || skipToNop) {
                    CodePoint cp = codePointFromEncodedChar(str[i]);
                    if (cp.inst == DUP && cp.val == 0) {
                        skipToNop = false;
                        prog->code[j] = (CodePoint){ .inst=NOP, .val=0 };
                        if (isVerbose()) {
                            printf("(%d, %d): inst: 0 val: 0\n", i, j);
                        }
                    } else {
                        prog->code[j] = cp;
                        if (isVerbose()) {
                            printf("(%d, %d): dat: %c%d => %d\n", i, j, instructionToChar(cp.inst), cp.val, (cp.inst << 4) | cp.val);
                        }
                    }
                    j++;
                    skipData--;
                    
                    // this looks weird... it turns out that our no-op instruction
                    // when processed from the raw bytes (which is a space)
                    // comes out as a DUP3 instruction
                    // THIS IS A NOP:

                    
                   
                    continue;
                }
                errno = 0;
                Instruction inst = charToInstruction(str[i]);
                const char maybeVal[2] = { str[i+1] };
                int val = (unsigned int)strtol(maybeVal, NULL, 16);
                
                if (str[i+1] == '+' || str[i+1] == '-') {
                    // there is a weird edge case where you have an
                    // addition operator directly after another instruction
                    // the strtol is totally okay with just a unary '+'
                    // but we don't consider that a number
                    // even if it *is* a number, we don't want it parsed
                    val = defaultForInstruction(inst);
                } else if (errno == 0) {
                    // no error on conversion means we found a value
                    // no increment the parsing position
                    i++;
                } else {
                    // otherwise, go looking for a reasonable default value
                    val = defaultForInstruction(inst);
                }

                if (inst == DAT) {
                    if (val == 0) {
                        skipToNop = true;
                    } else {
                        skipData = val;
                    }
                }
                if (isVerbose()) {
                    printf("(%d, %d): inst: %c val: %d\n", i, j, instructionToChar(inst), val);
                }
                prog->code[j] = (CodePoint) {.inst=inst, .val=val };
                j++;
            }
        }
    }
    prog->code[j] = (CodePoint){ .inst=0, .val=0 };
    
    return prog;
}
