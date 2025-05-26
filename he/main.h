//
//  main.h
//  Hackers Evolution
//
//  Created by Eric Baur on 5/17/25.
//

#include "helc.h"

const char *generateBytes(size_t num_bytes);

bool isVerbose(void);
bool isQuiet(void);

void runRepl(Program *prog, Tape *tape);
void printUsage(void);
