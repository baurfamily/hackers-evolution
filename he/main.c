//
//  main.c
//  he
//
//  Created by Eric Baur on 5/13/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include <time.h>

#include "main.h"

#define MAX_INPUT_SIZE 1024

/* Flag set by ‘--verbose’. */
static int verbose_flag;

/* Flag set by '--quiet' */
static int quiet_flag;

/* Flag set by ‘--byte-encoded’. */
static int byte_encoded;

int main(int argc, const char *argv[]) {
    int c;
    int numBytes;
    unsigned int seed = 0;
    bool repl = false;
    
    int executeCount = 1;
    
    Program *prog = NULL;
    
    while (1)
    {
        static struct option long_options[] =
        {
            /* These options set a flag. */
            {"verbose",     no_argument,       &verbose_flag, 1},
            {"byte-encoded",no_argument,       &byte_encoded, 'b'},
            {"quiet",       no_argument,       &quiet_flag, 'q'},
            /* These options don’t set a flag.
             We distinguish them by their indices. */
            {"version",     no_argument,       0, 'v'},
            {"interactive", no_argument,       0, 'i'},
            {"seed",        required_argument, 0, 's'},
            {"recurse",     required_argument, 0, 'R'},
            {"random",      required_argument, 0, 'r'},
            {"execute",     required_argument, 0, 'e'},
//            {"file",        required_argument, 0, 'f'},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;
        
        c = getopt_long (argc, argv, "vbiqs:r:R:e:",
                         long_options, &option_index);
        
        /* Detect the end of the options. */
        if (c == -1)
            break;
        
        switch (c)
        {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (long_options[option_index].flag != 0)
                    break;
                printf ("option %s", long_options[option_index].name);
                if (optarg)
                    printf (" with arg %s", optarg);
                printf ("\n");
                break;
                
            case 'i':
                repl = true;
                break;
                
            case 'R':
                executeCount = (unsigned int)strtol(optarg, NULL, 10);
                break;
                
            case 's':
                seed = (unsigned int)strtol(optarg, NULL, 10);
                break;
                
            case 'r':
                if (seed == 0) {
                    seed = (unsigned int)time(NULL) * getpid();
                }
                srand(seed);
                if (isVerbose()) {
                    printf("seed: %d", seed);
                }
                numBytes = (unsigned int)strtol(optarg, NULL, 10);
                prog = progFromBytes(generateBytes(numBytes));
                break;
                
            case 'e':
                if (isVerbose()) printf("Given prog: %s", optarg);
                
                if (byte_encoded) {
                    prog = progFromBytes(optarg);
                } else {
                    prog = progFromString(optarg);
                }
                break;
                
                // file option not supported yet
//            case 'f':
//                printf ("option -f with value `%s'\n", optarg);
//                break;
//                
            case '?':
                /* getopt_long already printed an error message. */
                break;
                
            default:
                abort ();
        }
    }
    
    if (verbose_flag)
        puts ("verbose flag is set");
    
    /* Print any remaining command line arguments (not options). */
    if (optind < argc)
    {
        printf ("non-option ARGV-elements: ");
        while (optind < argc)
            printf ("%s ", argv[optind++]);
        putchar ('\n');
    }
    
    for (int i=0; i<executeCount; i++) {
        Tape *tape = newTape();
        if (prog != NULL) {
             tape = tapeFromExecution(prog, NULL);
            
            if (repl) {
                runRepl(prog, tape);
            }
        } else if (repl) {
            runRepl(prog, tape);
        }
        if (!isQuiet()) printProg(prog);
        
        free(prog);
        
        if (executeCount > 1) {
            char newText[PROG_SIZE];
            for (int i=0; i<PROG_SIZE; i++) {
                newText[i] = tape->values[i] % 256;
            }
            prog = progFromBytes(newText);
            if (progIsEmpty(*prog)) {
                if (!isQuiet()) {
                    printf("Resulting program from tape is empty. Skipping recursion. (made %d passes, starting with seed %d)", i+1, seed);
                    break;
                }
            } else {
                if (!isQuiet()) {
                    printf("Reusing tape as new program: %d", i+1);
                }
            }
        }
    }

    
    printf("\n");
    exit (0);
}

const char *generateBytes(size_t num_bytes) {
    unsigned char *stream = malloc (num_bytes);
    size_t i;
    
    for (i = 0; i < num_bytes; i++) {
        stream[i] = rand ();
    }
 
    return stream;
}

bool isVerbose(void) {
    return verbose_flag;
}

bool isQuiet(void) {
    return quiet_flag;
}

void runRepl(Program *prog, Tape *tape) {
    printf("\nhelc version 0.0.1\n");
    printf("-- Interactive mode --\n");
    
    char input[MAX_INPUT_SIZE];

    if (prog == NULL) {
        prog = newProg();
    }
    if (tape == NULL) {
        tape = newTape();
    }
    
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
        
        for (int i=0; i<PROG_SIZE; i++) {
            CodePoint cp = inputProg->code[i];
            if (cp.inst != NOP) {
                prog->code[prog->pos+i] = cp;
//                printf("wrote instruction: %d => ( %c, %d )", prog->pos, instructionToChar(cp.inst), cp.val);
            }
        }
        
        int returnCode = -1;
        while (returnCode) {
            returnCode = step(prog, tape, 0);
        }
        if (!isQuiet()) printProg(prog);
        
//        printTape(*tape);
        printf("\n");
    }
}
