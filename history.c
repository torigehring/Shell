// [Assign3-cs240-msh-history.c]

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "history.h"

//#define DEBUG_ENABLE
#include "debug.h"

#define HISTORY_DEPTH 20

typedef struct node{
    history_entry_t data;
    struct node* next;
} node_t;

static node_t* g_head = NULL; 
static node_t* g_tail = NULL;

int g_history_count = 0;

// returns the last command from the history list
history_entry_t* last_command(){
    debug("----finding last command\n");
    history_entry_t* last_command;
    last_command = history_entry_lookup((g_history_count - 2));
    return last_command;
}

// deletes a history entry from the head of the list
int delete_from_head(void){
    node_t* delete_node;
    delete_node = g_head;
    g_head = g_head->next;
    int i;
    for(i = 0; i < delete_node->data.argc; i++){
        free(delete_node->data.argv[i]);
    }
    free(delete_node->data.argv);
    free(delete_node);
    return 0;
}

// 0 for success, -1 failure
// adds history entry to end and 
int add_history_entry(char ***argvp, int argc){
    debug("allocating a node\n");
    node_t* node = (node_t*)malloc(sizeof(node_t));
    if(node == NULL){ 
        printf("ERROR allocating space\n");
        return -1;
    }
    node->next = NULL;
    node->data.history_index = 0;
    node->data.argv = NULL;
    node->data.argc = argc;

    // Local argv is what argvp points to
    char** argv = *argvp;
    
    // Make a temp instance of struct and allocate space
    debug("allocating data.argv argc = %d\n", argc);
    node->data.argv = (char **)malloc((argc+1)*sizeof(char*));
    if(node->data.argv == NULL){
        printf("Error in allocation\n");
        return -1;
    }
    // Always overallocate 1 argv slot and set to NULL for execv
    node->data.argv[argc] = NULL;

    int i;
    for(i = 0; i < argc; i++){
        debug("argv[%d] = %s\n", i, argv[i]);
        node->data.argv[i] = (char *)malloc(sizeof(char)*80);
        if(node->data.argv[i] == NULL){
            printf("Error in allocation\n");
            return -1;
        }

        if(strlen(argv[i]) > 80){
            printf("Argument too long\n");
            return -2;
        }
        debug("strcpy function\n");
        strcpy(node->data.argv[i], argv[i]);
    }

    // free in reverse order or memory leak
    node->data.history_index = g_history_count;
 
    g_history_count++; // add one to total history
    
    if(g_head == NULL){
        g_head = node;
        g_tail = node;
    }else{
        g_tail->next = node;
        g_tail = node;
    }

    if(g_history_count > HISTORY_DEPTH){
        delete_from_head();
    }

    return 0;
}

// print list of history 
void print_history_entry(){
    node_t* temp_node;
    temp_node = g_head;
    while(temp_node != NULL){
        printf("%d: ", temp_node->data.history_index);
        int i;
        for(i = 0; i < temp_node->data.argc; i++){
            printf("%s", temp_node->data.argv[i]);
        }
        printf("\n");
        temp_node = temp_node->next;
    }
}


// returns 0 for a match, returns -1 for no match
history_entry_t* history_entry_lookup(int lookup_history_count){
    debug("******Searhcing\n");
    history_entry_t* found_history_entry;
    node_t* p;
    p = g_head;
    while(p != NULL){
        if( p->data.history_index == lookup_history_count){
            found_history_entry = &p->data;
            return found_history_entry;
        }else{
            p = p->next;
        }
    }
    printf("Did not find command\n");
    return NULL;
}
