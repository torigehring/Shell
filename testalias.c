#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alias.h"
#include "argparse.h"

int main(){


    char **argv;
    int argc;
    printf("\033[0;31m");

    
    argc = makearg("ls -l", &argv);
    add_alias_entry("ll", &argv, argc);

    argc = makearg("pwd", &argv);
    add_alias_entry("printdirectory", &argv, argc);

    argc = makearg("grep -v foo file.txt", &argv);
    add_alias_entry("findfoo", &argv, argc);

    argc = makearg("cd $HOME/bin", &argv);
    add_alias_entry("gobin", &argv, argc);

    argc = makearg("cat /dev/log --verbose", &argv);
    add_alias_entry("catlog", &argv, argc);


    alias_entry_t* stored_command;

      
    print_alias_entry();
    
    stored_command = alias_entry_lookup("ll");
    if(stored_command == NULL){
        printf("Error finding command\n");
        return -1;
    }else{
        printf("stored command:");
        int i;
        for(i = 0; i < stored_command->argc; i++){
            printf(" %s", stored_command->argv[i]);
        }
        printf("\n");
    }
    
    printf("removing catlog\n");
    int remove_success;
    remove_success = remove_alias_entry("catlog");
    if(remove_success == -1){
        printf("Error deleting alias");
    }
    print_alias_entry();

    printf("removing findfoo\n");
    int remove_success2;
    remove_success2 = remove_alias_entry("findfoo");
    if(remove_success2 == -1){
        printf("Error deleting alias");
    }

    print_alias_entry();

    printf("removing ll\n");
    int remove_success3;
    remove_success3 = remove_alias_entry("ll");
    if(remove_success3 == -1){
        printf("Error deleting alias");
    }

    print_alias_entry();

    printf("adding catlog\n");
    argc = makearg("cat /dev/log --verbose", &argv);
    add_alias_entry("catlog", &argv, argc);

    print_alias_entry();

    printf("removing catlog\n");
    remove_success3 = remove_alias_entry("catlog");
    if(remove_success3 == -1){
        printf("Error deleting alias");
    }

    printf("removing gobin\n");
    remove_success3 = remove_alias_entry("gobin");
    if(remove_success3 == -1){
        printf("Error deleting alias");
    }

    printf("removing printdirectory\n");
    remove_success3 = remove_alias_entry("printdirectory");
    if(remove_success3 == -1){
        printf("Error deleting alias");
    }

    print_alias_entry();

    printf("adding catlog\n");
    argc = makearg("cat /dev/log --verbose", &argv);
    add_alias_entry("catlog", &argv, argc);

    print_alias_entry(); 

    return 0;
    
}

