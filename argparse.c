// [Assign3-cs240-msh-argparse.c]
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "argparse.h"

//#define DEBUG_ENABLE
#include "debug.h"

// Calculate num arguments by spaces and ignoring spaces within quotes
int numArgs(char *s){
    int argc = 1;
    int quoteFlag = 0;
    int i = 0;
    while(s[i] != '\0'){
        if( (s[i] == ' ') && (quoteFlag == 0) ){
            argc++;
        }else{
            if(s[i] == '\'' || s[i] == '\"'){
                if(quoteFlag == 0){
                    quoteFlag = 1;
                }else{
                    quoteFlag = 0;
                }
            }
        }
        i++;
    }
    return argc;
}

// Returns newly allocated string that deletes leading/trailing/extra whitespaces
char * removeWhiteSpace(char *s){
    int len = strlen(s);
    char *newString = (char *)malloc(sizeof(char)*(len+1));

    // Find start
    int start = 0;
    while(s[start] == ' '){
        start++;
    }

    // Find end
    int end = (len - 1);
    while(s[end] == ' '){
        end--;
    }

    // Copy string while deleting extra spaces in the middle
    int newIndex = 0;
    int origIndex = start;
    for( origIndex = start; origIndex <= end; origIndex++){
        if ( !((s[origIndex] == ' ') && (s[origIndex-1] == ' ')) ){
            newString[newIndex] = s[origIndex];
            newIndex++;
        }
    }
    
    // NULL terminate after last character
    newString[newIndex] = '\0';

    return newString;
}

// split input string by commands seperated by semicolons
int split_by_semicolon(char *s, char ***args){
    char **comv; // command vector
    int comc; // command count
    int num_semicolons = 0;
    int len = strlen(s);
    int i;
    for(i = 0; i < len; i++){ 
        if(s[i] == ';'){
            num_semicolons++;
            if(i == (len - 1) || (i == 0)){
                num_semicolons--;
            }
        }
    }
    comc = (num_semicolons + 1);
    if(comc == 0){ return -1;} // If there are zero commands
    comv = (char**)malloc(comc*sizeof(char*));

    // Add elements to comv
    int start = 0;
    int comLen = 0;
    int sIndex;
    int comIndex = 0;
    for(sIndex = 0; sIndex <= len; sIndex++){
        if((s[sIndex] != ';') && (s[sIndex] != '\0')){
            comLen++;
        }else{
            if(sIndex != 0){
                char *tempCom = (char *)malloc(sizeof(char)*(comLen+1));
                int f;
                for(f = 0; f < comLen; f++){
                    tempCom[f] = s[start+f];
                }
                tempCom[comLen] = '\0';
                debug("command[%d]: %s\n", comIndex, tempCom);
                comv[comIndex] = tempCom;
                comIndex++;
            }
            start = sIndex+1;
            comLen = 0;
        }
    }
    // Return pointer to newly allocated memory holding arguments
    *args = comv;
    return comc;
}

// split by equals with split by semicolon code
int split_by_equals(char *s, char ***args){
    char **wordv; // command vector
    int wordc; // command count
    int num_equals = 0;
    int len = strlen(s);
    int i;
    for(i = 0; i < len; i++){ 
        if(s[i] == '='){
            num_equals++;
            if(i == (len - 1) || (i == 0)){
                num_equals--;
            }
        }
    }
    debug("Num equals = %d\n", num_equals);
    if(num_equals == 0){
        return -1; // no equals found
    }
    wordc = (num_equals + 1);
    if(wordc == 0){ return -1;} // If there are zero commands
    wordv = (char**)malloc(wordc*sizeof(char*));

    // Add elements to comv
    int start = 0;
    int wordLen = 0;
    int sIndex;
    int wordIndex = 0;
    for(sIndex = 0; sIndex <= len; sIndex++){
        if( (s[sIndex] == '=') || (s[sIndex] == '\0') ){
            if(sIndex != 0){
                char *tempWord = (char *)malloc(sizeof(char)*(wordLen+1));
                int f;
                for(f = 0; f < wordLen; f++){
                    tempWord[f] = s[start+f];
                }
                tempWord[wordLen] = '\0';
                debug("word[%d]: %s\n", wordIndex, tempWord);
                wordv[wordIndex] = tempWord;
                wordIndex++;
            }
            start = sIndex+1;
            wordLen = 0;
        }else{
            wordLen++;
        }
    }
    // Return pointer to newly allocated memory holding arguments
    *args = wordv;
    return wordc;
}

// Input is a string and fills array of strings and the last entry is NULL and returns argument count
int makearg(char *s, char ***args){ // update num spaces to calculate true argc
    // IF s[index] == space and not in doubleQuoted region make argv
    // ELSE if it is in double quoted region and is a space ignore and accumulate wordLen
    // Allocate space for argv
    char **argv;
    int argc = (numArgs(s));
    debug("makearg: argc = %d\n", argc);
    if(argc == 0){ return -1;} // If there are zero arguments
    argv = (char**)malloc((argc+1)*sizeof(char*)); // this will overallocate +1 for NULL
    int r;
    for(r = 0; r < (argc+1); r++){ // Init argv
        argv[r] = NULL;
    }
    // Add elements to argv
    int start = 0;
    int wordLen = 0;
    int sIndex;
    int argIndex = 0;
    int len = strlen(s);
    int doubleQuoteFlag = 0;

    for(sIndex = 0; sIndex <= len; sIndex++){
        debug(" - makearg: %c\n", s[sIndex]);
        if( (s[sIndex] == ' ' || s[sIndex] == '\0' ) && (doubleQuoteFlag == 0) ){
            char *tempWord = (char *)malloc(sizeof(char)*(wordLen+1)); 
            int f;
            for(f = 0; f < wordLen; f++){
                tempWord[f] = s[start+f];
            }
            tempWord[wordLen] = '\0';
            debug("argv[%d] = %s\n", argIndex, tempWord);
            argv[argIndex] = tempWord;
            start = sIndex+1;
            argIndex++;
            wordLen = 0;
            doubleQuoteFlag = 0;
            debug("argv[%d] = %s, start = %d argIndex = %d, wordLen = %d\n", argIndex, argv[argIndex], start, argIndex, wordLen);
        }else{
            if(s[sIndex] == '\"' || s[sIndex] == '\''){
                if(doubleQuoteFlag == 1){
                    doubleQuoteFlag = 0;
                }else{
                    doubleQuoteFlag = 1;
                }
            }
            wordLen++;
        }
    }
    debug("Completed makearg - found %d args\n", argc);
    int i;
    for(i = 0; i<argc; i++){
        debug(" %d: %s\n", i, argv[i]);
    }
    // Return pointer to newly allocated memory holding arguments
    *args = argv;
    return argc; 
}

// input string allocated by user, output string allocated by function
void strip_quotes(char *s, char **newS){
    int len = strlen(s);
    char *newString;
    int numQuotes = 0;
    int i;
    for(i = 0; i < len; i++){
        if(s[i] == '\"'){
            numQuotes++;
        }
    }
    newString = (char*)malloc(((len - numQuotes)+2)*sizeof(char)); 
    int j;
    int newStringIndex = 0;
    for(j = 0; j < len; j++){
        if(s[j] != '\"'){
            newString[newStringIndex] = s[j];
            newStringIndex++;
        }
    }
    newString[newStringIndex] = '\0'; // null terminate last index
    *newS = newString;
}

// make an array of commands seperated by the | symbol, return amount of commands to be piped
int split_by_pipe(char *s, char ***args){
    char **pipev; // pipe vector
    int pipec; // pipe count
    int num_pipes = 0;
    int len = strlen(s);
    int i;
    for(i = 0; i < len; i++){ 
        if(s[i] == '|'){
            num_pipes++;
            if(i == (len - 1) || (i == 0)){
                num_pipes--;
            }
        }
    }
    pipec = (num_pipes + 1);
    if(pipec == 0){ return -1;} // If there are zero commands
    pipev = (char**)malloc(pipec*sizeof(char*));

    // Add elements to comv
    int start = 0;
    int pipeLen = 0;
    int sIndex;
    int pipeIndex = 0;
    for(sIndex = 0; sIndex <= len; sIndex++){
        if((s[sIndex] != '|') && (s[sIndex] != '\0')){
            pipeLen++;
        }else{
            if(sIndex != 0){
                char *tempPipe = (char *)malloc(sizeof(char)*(pipeLen+1));
                int f;
                for(f = 0; f < pipeLen; f++){
                    tempPipe[f] = s[start+f];
                }
                tempPipe[pipeLen] = '\0';
                debug("command[%d]: %s\n", pipeIndex, tempPipe);
                pipev[pipeIndex] = tempPipe;
                pipeIndex++;
            }
            start = sIndex+1;
            pipeLen = 0;
        }
    }
    // Return pointer to newly allocated memory holding arguments
    *args = pipev;
    return pipec;
}
