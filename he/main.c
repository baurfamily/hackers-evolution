//
//  main.c
//  he
//
//  Created by Eric Baur on 5/13/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

#define MAX_INPUT_SIZE 1024

int main(int argc, const char * argv[]) {
    // Example: Processing a specific argument
    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            printUsage();
        } else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
            printf("Program Version 0.0.1\n");
        } else if (strcmp(argv[1], "-e") == 0 || strcmp(argv[1], "--execute") == 0) {
            if (argc < 2) {
                printf("Not enough arguments for execute, please provide the code to execute after option.");
                return 1;
            }
            Program *prog = progFromString(argv[2]);
            execute(prog);
            free(prog);
        } else if (strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "--interactive") == 0) {
            runRepl();
        } else {
            printUsage();
        }
    } else {
        printUsage();
    }
    
    return 0;
}

void printUsage(void) {
    printf("Usage: program_name [options] [arguments]\n");
    printf("Options:\n");
    printf("  -h, --help     Display this help message\n");
    printf("  -v, --version  Display version information\n");
    printf("  -e,  --execute Execute program specified on the commandline.");
    printf("  -i,  --interactive  Run a REPL you can interact with.\n\n");
}

void runRepl(void) {
    printf("helc version 0.0.1\n");
    printf("-- Interactive mode --\n");
    
    char input[MAX_INPUT_SIZE];

    Program *prog = newProg();
    Stack *stack = newStack();
    
    while (1) {
        printf("> ");
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            printf("\nExiting...\n");
            break;
        }

        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) {
            printf("Exiting...\n");

            // this doens't seem to print the whole thing
            printProg(prog);
            printf("\n");
            break;
        }
        
        
        Program *inputProg = progFromString(input);
        
        // if there was only one instruction with no
        // value specified, use 1 - it's a common enough default
        if (strlen(input) == 1) { inputProg->code[0].val = 1; }
        
        int returnCode = -1;
        while (returnCode) {
            prog->code[prog->pos] = inputProg->code[inputProg->pos];
            prog->pos = prog->pos + 1;
            returnCode = step(inputProg, stack, 0);
            // #A(0#1%1,1)0#2
            
//            printf("%d => [ ", returnCode);
//            printStack(*stack);
//            printf("]\n\n");
        }
        printf("[ ");
        printStack(*stack);
        printf("]\n\n");
    }
    
}
