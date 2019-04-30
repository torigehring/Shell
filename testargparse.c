#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "argparse.h"

int main(){
    /*
    printf("Test argparse\n");
    int numArgs = numSpaces("alias hi ll=\"hi\" ll=\"ls -l\"");
    printf("Number args = %d\n", numArgs);

    
    char **comv;
    int comc;
    comc = split_by_semicolon("alias hi ll=\"ls -l\" ll=\"ls -l\"", &comv);
    printf("comc: %d\n", comc);
    int i;
    for(i = 0; i < comc; i++){
        printf("Command [%d]: %s\n", i, comv[i]);
    }
    */
    char **pipev;
    int pipec;
    pipec = split_by_pipe(" ls | more", &pipev);
    printf("pipec: %d\n", pipec);
    int i;
    for(i = 0; i < pipec; i++){
        printf("Pipe split [%d]: %s\n", i, pipev[i]);
    }
    /*
    // make arguments from commands
    int argc;
    char** argv;
    int m;
    for(m = 0; m < comc; m++){
        printf("COMMAND %d: \n", m);
        argc = makearg(comv[m], &argv);
        printf("Argc = %d\n", argc);
        int j;
        for(j = 0; j < argc; j++){
            printf("Arg[%d] = %s\n", j, argv[j]);
        }
    }

    // vector with each side of equals returned (in case of alias)
    // ll="ls -l" is returned as wordv[0] == "ll"
    //                           wordv[1] == "ls-l"
    printf("Splitting alias\n");
    int wordc;
    char **wordv;
    int k;
    for(k = 0; k < argc; k++){
        printf("Argv[%d] = %s\n", k, argv[k]);
        wordc = split_by_equals(argv[k], &wordv);
        int n;
        for(n = 0; n < wordc; n++){
            printf("ALIAS[%d]: %s\n", n, wordv[n]);
        }
    }

    // strip quotes off of alias[1] = "ls -l" -> ls -l
    char *newS;
    strip_quotes(wordv[1], &newS);
    printf("new String: %s\n", newS);
    */
    return 0;
}