//
//  instructions.h
//  Hackers Evolution
//
//  Created by Eric Baur on 5/15/25.
//

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#define MOVE_PROG(val) prog->pos = PROG_DELTA(val);
#define PROG_DELTA(val) ((PROG_SIZE + prog->pos + val) % PROG_SIZE)
#define CURRENT_PROG (prog->code[prog->pos])

#define TAPE_DELTA(val) ((TAPE_SIZE + tape->pos + val) % TAPE_SIZE)
#define MOVE_TAPE(val) tape->pos = TAPE_DELTA(val)
#define CURRENT_VALUE (tape->values[tape->pos])
#define PREVIOUS_VALUE (tape->values[TAPE_DELTA(-1)])
#define VALUE_AT(val) (tape->values[TAPE_DELTA(val)])
// implicitly subtracts
#define PREVIOUS_VALUE_AT(val) (tape->values[TAPE_DELTA(-(val))])


void instNOP(int val, Program *prog, Tape *tape);
void instRED(int val, Program *prog, Tape *tape);
int findDAT(Program *prog);
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
