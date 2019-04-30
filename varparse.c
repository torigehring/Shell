// [Assign3-cs240-msh-varparse.c]
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "varparse.h"
#include "argparse.h"

//#define DEBUG_ENABLE
#include "debug.h"

typedef struct node{
    var_entry_t data;
    struct node* next;
} node_t;

static void free_node(node_t* delete_node);

static node_t* g_head = NULL;
static node_t* g_tail = NULL;
static int max_name_len = 80;
static int max_value_len = 2048;

// evaluate the value of a string if it has $ENVVAR (environment variables) in it
static char *evaluate(char *value){
    debug("Enter evaluate(%s)\n", value);
    int i_len = strlen(value);
    int o_len = max_value_len; // output length
    char *output = (char *)malloc(o_len*sizeof(char)+1); // overallocate for variable substitutions
    if (output == NULL) {
        printf("Error: Cannot allocate space for output sting\n");
        return NULL;
    }
    int i;
    int o = 0;
    int start = 0;
    int var_name_len;
    int is_var_flag = 0;
    for(i = 0; i <= i_len; i++){
        if(value[i] == '$'){
            debug("found start of var at %d\n", i);
            start = i+1;
            var_name_len = 0;
            is_var_flag = 1;
            continue;
        }
        if(is_var_flag == 1){
            if((isupper(value[i]) || (value[i]) == '_')){
                var_name_len++;
                debug("var_name_len = %d\n", var_name_len);
            }else{
                is_var_flag = 0;
                char* lookup_name = (char *)malloc((var_name_len +1)*sizeof(char));
                int j;
                for(j = 0; j < var_name_len; j++){
                    lookup_name[j] = value[start + j]; 
                }
                lookup_name[var_name_len] = '\0';
                debug("variable name: [%s]\n", lookup_name);
                var_entry_t* lookup_result;
                lookup_result = var_entry_lookup(lookup_name); // lookup existing variable
                debug("lookup result: [%s]\n", lookup_result->value);
                if(lookup_result != NULL){
                    // if found copy value into output string
                    int replace_len = strlen(lookup_result->value);
                    int k;
                    for(k = 0; k < replace_len; k++){
                        output[o] = lookup_result->value[k];
                        o++;
                        if(o > o_len){
                            printf("didn't allocate enough space for output value\n");
                        }
                    }
                    output[o] = value[i];
                    o++;
                }else{ 
                    // if not found print error
                    printf("ERROR: variable not found\n");
                    return NULL;
                }
            }
        }else{
            // for normal characters
            output[o] = value[i];
            o++; 
            if(o > o_len){
                printf("didn't allocate enough space for output value\n");
            }
        }
    }
    output[o] = '\0';
    return output;
}

// add over existing var if it matches one
var_entry_t* add_var_entry(char* name, char* value){
    //remove_var_entry(var);
    debug("add_var_entry: name='%s', value='%s'\n", name, value);
    debug(" - value=[%s]\n", value);
    node_t* node = (node_t*)malloc(sizeof(node_t));
    if(node == NULL){ 
        printf("ERROR allocating space\n");
        return NULL;
    }
    debug(" - value=[%s]\n", value);
    node->next = NULL;
    node->data.name = NULL;
    node->data.value = NULL;
 
    // Make a temp instance of struct and allocate space for name and value
    node->data.name = (char *)malloc(max_name_len*sizeof(char*));
    if(node->data.name == NULL){
        printf("Error in allocation\n");
        return NULL;
    }

    node->data.value = (char *)malloc(max_value_len*sizeof(char*));
    if(node->data.value == NULL){
        printf("Error in allocation\n");
        return NULL;
    }

    // Always Null terminate and check size of inputs, then copy them into struct
    node->data.name[0] = '\0';
    node->data.value[0] = '\0';
    if(strlen(name) > max_name_len){
        printf("Argument too long\n");
        return NULL;
    }
    if(strlen(value) > max_value_len){
        printf("Argument too long\n");
        return NULL;
    }
    
    strcpy(node->data.name, name);
    debug("Calling evaluate [%s]\n", value);
    char *evaluated_value = evaluate(value); // storing evaluated value
    debug("Evaluated value: [%s]\n", evaluated_value);
    strcpy(node->data.value, evaluated_value);

    // lookup the name to see if it already exists
    var_entry_t* lookup_result = var_entry_lookup(name);
    if(lookup_result != NULL){
        debug("replacing existing variable %s\n", name);
        remove_var_entry(name);
    }

    // place the newly allocated node in the list
    if(g_head == NULL){
        g_head = node;
        g_tail = node;
    }else{
        g_tail->next = node;
        g_tail = node;
    }

    return &node->data;
}

// print the environment variables list
void print_var_entry(){
    printf("____VARIABLE LIST:___\n");
    node_t* temp_node;
    temp_node = g_head;
    while(temp_node != NULL){
        printf("var[%s] = [%s]\n", temp_node->data.name, temp_node->data.value);
        temp_node = temp_node->next;
    }
}

// returns 0 for a match, returns -1 for no match
var_entry_t* var_entry_lookup(char *name){
    debug("******Searching for %s\n", name);
    var_entry_t* search_var_entry;
    node_t* p;
    p = g_head;
    while(p != NULL){
        if(strcmp(p->data.name, name) == 0 ){
            search_var_entry = &p->data;
            return search_var_entry;
        }else{
            p = p->next;
        }
    }
    debug("Did not find variable %s\n", name);
    return NULL;
}

// removes an environment variable by name (0 = success, -1 = failure/did not find entry)
int remove_var_entry(char* name){
    node_t* prev;
    node_t* curr;
    curr = g_head;
    prev = NULL;

    while(curr != NULL){
        // check for the tail
        if((curr->next == NULL) && (strcmp(curr->data.name, name) == 0)){
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
        if((prev == NULL) && (strcmp(curr->data.name, name) == 0)){
            g_head = curr->next;
            free_node(curr);
            return 0;
        }
        // normal, middle case
        if( strcmp(curr->data.name, name) == 0 ){
            prev->next = curr->next;
            free_node(curr);
            return 0;
        }
        prev = curr;
        curr = curr->next;
        
    }
    //printf("Did not find var\n");
    return -1;
}

// deallocates space for a node
static void free_node(node_t* delete_node){
    free(delete_node->data.name);
    free(delete_node->data.value);
    free(delete_node);
}


