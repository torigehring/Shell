#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "varparse.h"
#include "argparse.h"

int main(){
    add_var_entry("HELLO", "6");
    print_var_entry();

    add_var_entry("BYE", "buh-bye");
    print_var_entry();

    var_entry_t* stored_var;
    stored_var = var_entry_lookup("BYE");
    if(stored_var == NULL){
        printf("Error finding command\n");
        return -1;
    }else{
        printf("stored variable: %s = %s\n", stored_var->name, stored_var->value);
    }
    
    printf("removing HELLO\n");
    int remove_success;
    remove_success = remove_var_entry("HELLO");
    if(remove_success == -1){
        printf("Error deleting variable");
    }
    print_var_entry();

    printf("removing BYE\n");
    int remove_success2;
    remove_success2 = remove_var_entry("BYE");
    if(remove_success2 == -1){
        printf("Error deleting variable\n");
    }

    print_var_entry();

    add_var_entry("HELLO", "6");
    print_var_entry();

    add_var_entry("BYE", "FOO");
    print_var_entry();

    print_var_entry();

    add_var_entry("GOODBYE", "buh-$HELLO-bye-$BYE"); // account for end
    print_var_entry();

    add_var_entry("PATH", getenv("PATH"));
    print_var_entry();

    add_var_entry("PATH", "$PATH:/foo/bin/bla/longsuperlogname/thisisreallylong");
    print_var_entry();



    return 0;
    
}

