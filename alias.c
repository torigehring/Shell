// [Assign3-cs240-msh-alias.c]

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alias.h"

//#define DEBUG_ENABLE
#include "debug.h"

typedef struct node{
    // node->history_entry->argv[2];
    alias_entry_t data;
    struct node* next;
} node_t;

static void free_node(node_t* delete_node);

static node_t* g_head = NULL;
static node_t* g_tail = NULL;

// Alias over existing aliases if it matches one, otherwise just adds the alias
int add_alias_entry(char * alias, char ***argvp, int argc){
    char** argv = *argvp;
    remove_alias_entry(alias);
    debug("add_alias_entry: alias='%s', argc=%d:\n", alias, argc);
    int i;
    for(i = 0; i < argc; i++){
        debug(" - %d: %s\n", i, argv[i]);
    }
    node_t* node = (node_t*)malloc(sizeof(node_t));
    if(node == NULL){ 
        printf("ERROR allocating space\n");
        return -1;
    }
    node->next = NULL;
    node->data.alias[0] = '\0';
    node->data.argv = NULL;
    node->data.argc = argc;

    // Make a temp instance of struct and allocate space
    node->data.argv = (char **)malloc((argc+1)*sizeof(char*));
    if(node->data.argv == NULL){
        printf("Error in allocation\n");
        return -1;
    }
    // Always overallocate 1 argv slot and set to NULL for execv
    node->data.argv[argc] = NULL;

    for(i = 0; i < argc; i++){
        node->data.argv[i] = (char *)malloc(sizeof(char)*80);
        if(node->data.argv[i] == NULL){
            printf("Error in allocation\n");
            return -1;
        }
        if(strlen(argv[i]) > 80){
            printf("Argument too long\n");
            return -2;
        }
        strcpy(node->data.argv[i], argv[i]);
    }

    strcpy(node->data.alias, alias);
    
    if(g_head == NULL){
        g_head = node;
        g_tail = node;
    }else{
        g_tail->next = node;
        g_tail = node;
    }

    return 0;
}

// prints a list of existing aliases
void print_alias_entry(){
    printf("____ALIAS LIST:___\n");
    node_t* temp_node;
    temp_node = g_head;
    while(temp_node != NULL){
        printf("ALIAS[%s]:", temp_node->data.alias);
        int i;
        for(i = 0; i < temp_node->data.argc; i++){
            printf(" %s", temp_node->data.argv[i]);
        }
        printf("\n");
        temp_node = temp_node->next;
    }
}

// returns 0 for a match, returns -1 for no match
alias_entry_t* alias_entry_lookup(char *alias_lookup){
    debug("******Searching\n");
    alias_entry_t* search_alias_entry;
    node_t* p;
    p = g_head;
    while(p != NULL){
        if(strcmp(p->data.alias, alias_lookup) == 0 ){
            search_alias_entry = &p->data;
            return search_alias_entry;
        }else{
            p = p->next;
        }
    }
    debug("Did not find alias\n");
    return NULL;
}

// remove entry from alias list if it exists returns -1 for failure, 0 for success
int remove_alias_entry(char* remove_alias){
    node_t* prev;
    node_t* curr;
    curr = g_head;
    prev = NULL;

    while(curr != NULL){
        // check for the tail
        if((curr->next == NULL) && (strcmp(curr->data.alias, remove_alias) == 0)){
            g_tail = prev;
            if(prev != NULL){
                prev->next = NULL;
            }else{
                g_head = NULL;
            }
            free_node(curr); // free more
            return 0; 
        } 
        // check for head
        if((prev == NULL) && (strcmp(curr->data.alias, remove_alias) == 0)){
            g_head = curr->next;
            free_node(curr);
            return 0;
        }
        // normal, middle case
        if( strcmp(curr->data.alias, remove_alias) == 0 ){
            prev->next = curr->next;
            free_node(curr);
            return 0;
        }
        prev = curr;
        curr = curr->next;
        
    }
    //printf("Did not find alias\n");
    return -1;
}

static void free_node(node_t* delete_node){
    int i;
    for(i = 0; i < delete_node->data.argc; i++){
        free(delete_node->data.argv[i]);
    }
    free(delete_node->data.argv);
    free(delete_node);
}


