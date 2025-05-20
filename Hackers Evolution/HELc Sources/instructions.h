//
//  instructions.h
//  Hackers Evolution
//
//  Created by Eric Baur on 5/15/25.
//

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

void instNOP(int val, Program *prog, Tape *tape);
void instRED(int val, Program *prog, Tape *tape);
void instDUP(int val, Program *prog, Tape *tape);
void instINS(int val, Program *prog, Tape *tape);
void instOUT(int val, Program *prog, Tape *tape);
void instSWP(int val, Program *prog, Tape *tape);
void instAND(int val, Program *prog, Tape *tape);
void instINC(int val, Program *prog, Tape *tape);
void instANC(int val, Program *prog, Tape *tape);
void findEND(Program *prog);
void instEND(int val, Program *prog, Tape *tape);
void findANC(Program *prog);
void instMUL(int val, Program *prog, Tape *tape);
void instADD(int val, Program *prog, Tape *tape);
void instDEC(int val, Program *prog, Tape *tape);
void instSUB(int val, Program *prog, Tape *tape);
void instDAT(int val, Program *prog, Tape *tape);
void instDIV(int val, Program *prog, Tape *tape);

#endif
