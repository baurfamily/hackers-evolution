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

int step(Program *prog, Stack *stack) {
    prog->pos = prog->pos + 1;
    
    CodePoint code = prog->code[prog->pos];
    Instruction inst = code.inst;
    char val = code.val;
    
    if (inst==0) return 0;
    
    switch (inst) {
        case NOP: instNOP(val, prog, stack); break;
//            case RED: return '!';
        case DUP: instDUP(val, prog, stack); break;
        case INS: instINS(val, prog, stack); break;
        case OUT: instOUT(val, prog, stack); break;
        case SWP: instSWP(val, prog, stack); break;
//            case AND: return '&';
        case INC: instINC(val, prog, stack); break;
//            case ANC: return '(';
//            case END: return ')';
        case MUL: instMUL(val, prog, stack); break;
        case ADD: instADD(val, prog, stack); break;
        case DEC: instDEC(val, prog, stack); break;
        case SUB: instSUB(val, prog, stack); break;
//            case DAT: return '.';
        case DIV: instDIV(val, prog, stack); break;
        default:  printf("unmatched instruction\n");
    }
    
    return inst;
}

void executeWithStack(Program *prog, Stack *stack) {
    printf("\nProgram execution!\n");
    printProg(prog);
    
    // init everything
    for (int i=0; i<PROG_SIZE; i++) {
        stack->values[i] = 0;
        stack->pos = -1;
    }
    
    for (int i=0; i<PROG_SIZE; i++) {
        int returnCode = step(prog, stack);
        if (returnCode==0) break;
        
        printStack(*stack);
    }
}

void execute(Program *prog) {
    printf("\nProgram execution!\n");
    printProg(prog);
    
    Stack stack = { .values={}, .pos=-1 };
    executeWithStack(prog, &stack);
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
    prog->pos = -1;
    
    // initialize array to null
    for (int i=0; i<PROG_SIZE; i++) {
        prog->code[i] = (CodePoint){ .inst=0, .val=0 };
    }
    
    return prog;
}

Stack* newStack(void) {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->pos = -1;
    
    for (int i=0; i<PROG_SIZE; i++) {
        stack->values[i] = 0;
    }
    
    return stack;
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

void printStack(Stack stack) {
    for (int i=stack.pos; i>=0; i--) {
        if (stack.values[i] == 0) continue;
        printf("%d ", stack.values[i]);
    }
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
    for (int i=0; i<length; i+=2) {
        prog->code[j] = (CodePoint) {
            .inst = charToInstruction(str[i]),
            .val = (unsigned int)strtol(&str[i+1], NULL, 16)
        };
        j++;
    }
    prog->code[j] = (CodePoint){ .inst=0, .val=0 };
    
    return prog;
}
