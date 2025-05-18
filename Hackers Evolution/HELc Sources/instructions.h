//
//  instructions.h
//  Hackers Evolution
//
//  Created by Eric Baur on 5/15/25.
//

void instNOP(char val, Program *prog, Stack *stack);
void instRED(char val, Program *prog, Stack *stack);
void instDUP(char val, Program *prog, Stack *stack);
void instINS(char val, Program *prog, Stack *stack);
void instOUT(char val, Program *prog, Stack *stack);
void instSWP(char val, Program *prog, Stack *stack);
void instAND(char val, Program *prog, Stack *stack);
void instINC(char val, Program *prog, Stack *stack);
void instANC(char val, Program *prog, Stack *stack);
void instEND(char val, Program *prog, Stack *stack);
void instMUL(char val, Program *prog, Stack *stack);
void instADD(char val, Program *prog, Stack *stack);
void instDEC(char val, Program *prog, Stack *stack);
void instSUB(char val, Program *prog, Stack *stack);
void instDAT(char val, Program *prog, Stack *stack);
void instDIV(char val, Program *prog, Stack *stack);
