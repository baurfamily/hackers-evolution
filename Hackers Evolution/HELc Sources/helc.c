//
//  helc.c
//  Hackers Evolution
//
//  Created by Eric Baur on 5/11/25.
//

#include "helc.h"
#include "instructions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void execute(CodePoint *code) {
    printf("\nProgram execution!\n");
    
    for (int i=0; i<PROG_SIZE; i++) {
        
    }
    
    printProg(code);
    
}

Instruction charToInstruction(char c) {
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

CodePoint* newProg(void) {
    CodePoint *code = (CodePoint *)malloc(sizeof(CodePoint)*(PROG_SIZE+1));
    
    // initialize array to null
    for (int i=0; i<PROG_SIZE; i++) {
        code[i] = (CodePoint){ .inst=0, .val=0 };
    }
    
    return code;
}

Instance* newInstance(void) {
    Instance *instance = (Instance *)malloc(sizeof(Instance));
    
    // init everything
    for (int i=0; i<PROG_SIZE; i++) {
        instance->prog.code[i] = (CodePoint){ .inst=0, .val=0 };
        instance->prog.pos = -1;
        instance->stack.values[i] = 0;
        instance->stack.pos = -1;
    }
    
    return instance;
}

void printProg(CodePoint *program) {
    char str[PROG_SIZE*2 + 1];
    char val[PROG_SIZE*2 + 1];
    
    for (int i=0; i<PROG_SIZE; i++) {
        Instruction inst = program[i].inst;
        char c = instructionToChar(inst);
        str[i] = c;
        val[i] = (int)program[i].val;
        if (inst == 0)
            break;
    }
    
    for (int i=0; i<PROG_SIZE; i++) {
        if (program[i].inst == 0) break;
        printf("%c%d ", str[i], val[i]);
    
        if ((i+1)%8 == 0) printf("\n");
        
        if (program[i].inst == 0) break;
    }
}

CodePoint* progFromString(char *str) {
    CodePoint *code = newProg();
    
    unsigned long length = strlen(str);

    int j=0;
    for (int i=0; i<length; i+=2) {
        code[j] = (CodePoint) {
            .inst = charToInstruction(str[i]),
            .val = (unsigned int)strtol(&str[i+1], NULL, 16)
        };
        j++;
    }
    code[j] = (CodePoint){ .inst=0, .val=0 };
    
    return code;
}
