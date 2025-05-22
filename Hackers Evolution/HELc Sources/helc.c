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

#include "helc.h"
#include "instructions.h"

int step(Program *prog, Tape *tape, int additionalVal) {
    CodePoint code = CURRENT_PROG;
    Instruction inst = code.inst;
    unsigned int val = code.val + (additionalVal << 4);
    
    printf("%02d: %c%d => ", prog->pos, instructionToChar(code.inst), val);
    if (inst==0) return 0;
    
    switch (inst) {
        case NOP: instNOP(val, prog, tape); break;
//            case RED: return '!';
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
//            case DAT: return '.';
        case DIV: instDIV(val, prog, tape); break;
        default:  printf("unmatched instruction\n");
    }
    MOVE_PROG(1);
    
    printTape(*tape);
    
    return inst;
}

void executeWithTape(Program *prog, Tape *tape) {
    printf("\n");
    printProg(prog);
    printf("\n");
    
    // init everything
    for (int i=0; i<PROG_SIZE; i++) {
        tape->values[i] = 0;
        tape->pos = 0;
    }
    
    for (int i=0; i<MAX_EXECUTION; i++) {
        int returnCode = step(prog, tape, 0);
        if (returnCode==0) break;
    }
}

void execute(Program *prog) {
    Tape tape = { .values={}, .pos=0 };
    executeWithTape(prog, &tape);
}

Tape* tapeFromExecution(Program *prog) {
    Tape *tape = newTape();
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
        case RED: return 1;
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
        case DAT: return 1;
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
    printf("[... ");
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
    printf("...]\n");
}

void printProg(Program *prog) {
    char str[PROG_SIZE*2 + 1];
    char val[PROG_SIZE*2 + 1];
    
    for (int i=0; i<PROG_SIZE; i++) {
        Instruction inst = prog->code[i].inst;
        char c = instructionToChar(inst);
        str[i] = c;
        val[i] = (int)prog->code[i].val;
        if (inst == 0)
            break;
    }
    
    for (int i=0; i<PROG_SIZE; i++) {
        if (prog->code[i].inst == 0) break;
        printf("%c%d ", str[i], val[i]);
    
        if ((i+1)%8 == 0) printf("\n");
        
        if (prog->code[i].inst == 0) break;
    }
}

CodePoint codePointFromString(const char *str) {
    return (CodePoint) {
        .inst = charToInstruction(str[0]),
        .val = (unsigned int)strtol(&str[1], NULL, 16)
    };
}

Program* progFromString(const char *str) {
    Program *prog = newProg();
    
    unsigned long length = strlen(str);

    int j=0;
    bool comment = false;
    for (int i=0; i<length; i++) {
        if (str[i] == ':') {
            comment = true;
        } else if (str[i] == ';') {
            comment = false;
        } else {
            if (!comment) {
                char *endptr;
                errno = 0;
                Instruction inst = charToInstruction(str[i]);
                int val = (unsigned int)strtol(&str[i+1], &endptr, 16);
                
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
//                printf("(%d, %d): inst: %c val: %d\n", i, j, instructionToChar(inst), val);
                prog->code[j] = (CodePoint) {.inst=inst, .val=val };
                j++;
            }
        }
    }
    prog->code[j] = (CodePoint){ .inst=0, .val=0 };
    
    return prog;
}
