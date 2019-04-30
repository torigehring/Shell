// alias.h 

#ifndef ALIAS_H
#define ALIAS_H

typedef struct alias_entry 
{ 
    char alias[128];
    char **argv; 
    int argc;
} alias_entry_t; 

// Prototypes
int add_alias_entry(char * alias, char ***argvp, int argc);
void print_alias_entry();
// have this return null if nothing
alias_entry_t* alias_entry_lookup(char * alias);
int remove_alias_entry(char* remove_alias);

#endif