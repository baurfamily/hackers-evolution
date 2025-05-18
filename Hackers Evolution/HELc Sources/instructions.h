//
//  instructions.h
//  Hackers Evolution
//
//  Created by Eric Baur on 5/15/25.
//

void instNOP(int val, Program *prog, Stack *stack);
void instRED(int val, Program *prog, Stack *stack);
void instDUP(int val, Program *prog, Stack *stack);
void instINS(int val, Program *prog, Stack *stack);
void instOUT(int val, Program *prog, Stack *stack);
void instSWP(int val, Program *prog, Stack *stack);
void instAND(int val, Program *prog, Stack *stack);
void instINC(int val, Program *prog, Stack *stack);
void instANC(int val, Program *prog, Stack *stack);
void instEND(int val, Program *prog, Stack *stack);
void instMUL(int val, Program *prog, Stack *stack);
void instADD(int val, Program *prog, Stack *stack);
void instDEC(int val, Program *prog, Stack *stack);
void instSUB(int val, Program *prog, Stack *stack);
void instDAT(int val, Program *prog, Stack *stack);
void instDIV(int val, Program *prog, Stack *stack);
