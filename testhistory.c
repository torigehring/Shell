#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"
#include "argparse.h"

int main(){
    char **argv;
    int argc;

    argc = makearg("ls -l", &argv);
    add_history_entry(&argv, argc);

    argc = makearg("pwd", &argv);
    add_history_entry(&argv, argc);

     argc = makearg("grep -v foo file.txt", &argv);
    add_history_entry(&argv, argc);

     argc = makearg("cd $HOME/bin", &argv);
    add_history_entry(&argv, argc);

     argc = makearg("cat /dev/log --verbose", &argv);
    add_history_entry(&argv, argc);

     argc = makearg("ls -l", &argv);
    add_history_entry(&argv, argc);

     argc = makearg("ls -l", &argv);
    add_history_entry(&argv, argc);

    history_entry_t* stored_command;

      
    print_history_entry();

    stored_command = history_entry_lookup(55);
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
    print_history_entry();
    return 0;
}

