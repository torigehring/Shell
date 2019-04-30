// varparse.h 

#ifndef VARPARSE_H
#define VARPARSE_H

typedef struct var_entry 
{ 
    char* name;
    char *value; 
} var_entry_t; 

// Prototypes
var_entry_t* add_var_entry(char * name, char *value);
void print_var_entry();
// have this return null if nothing
var_entry_t* var_entry_lookup(char * name);
int remove_var_entry(char* name);

#endif