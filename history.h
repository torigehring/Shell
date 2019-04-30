// history.h
// This is a queue

#ifndef HISTORY_H
#define HISTORY_H

typedef struct history_entry
{ 
    int argc;
    char **argv; 
    int history_index;  
}history_entry_t; 

// Prototypes

// This function will enqueue and dequeue together.
int add_history_entry(char ***argv, int argc);
void print_history_entry();
// have this return null if nothing
// Lookup will have to print a count
history_entry_t* history_entry_lookup(int lookup_history_count);
history_entry_t* last_command();

#endif