// Victoria Gehring
// CS240 Assignment 3 (msh.c)
// [Assign3-cs240-msh-msh.c]

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "argparse.h"
#include "history.h"
#include "alias.h"
#include "varparse.h"
#include "pipeutil.h"

//#define DEBUG_ENABLE
#include "debug.h"


void execute(char **argv);
void process_command(char ***argvp, int argc);
void process_line(char *line);
void process_file(const char* filepath);
int file_exists(const char *filepath);


int main(void){

    printf("******COLOR OPTIONS******* \n");
    printf("Enter number for colored output or press enter to continue: \n");
    printf(" 1: RED     2: BLUE\n");
    printf(" 3: GREEN   4: YELLOW\n");
    printf(" 5: MAGENTA: 6: CYAN\n");
    printf(" 0: STANDARD COLOR\n");

    char colorBuffer[32];
    fgets(colorBuffer, 32, stdin);
    int colorChoice = atoi(colorBuffer);
    if(colorChoice == 1){
        printf("\033[0;31m");
    }else if(colorChoice == 2){
        printf("\033[0;34m");
    }else if(colorChoice == 3){
        printf("\033[0;32m");
    }else if(colorChoice == 4){
        printf("\033[0;33m");
    }else if(colorChoice == 5){
        printf("\033[0;35m");
    }else if(colorChoice == 6){
        printf("\033[0;36m");
    }


    // Init internal $PATH
	add_var_entry("PATH", getenv("PATH"));
		
    int len;
    int maxInput = 80;
    char inputLine[maxInput]; // Original input
	
	if(file_exists("mshrc") == 1){ // file exists
		printf("File exists\n");
		process_file("mshrc");
	}else{ // file doesn't exist
		printf("mshrc file does not exist\n");
	}

    while(1){ 
		// once mshrc runs or the file doesn't exist
		printf(">:");
		fgets(inputLine, maxInput, stdin);
		len = strlen(inputLine);
		inputLine[len-1] = '\0'; // terminate input with NULL
		debug("input line: %s\n", inputLine);
		char *newInputLine = removeWhiteSpace(inputLine);
		// detect pipes in user input
		int num_pipes = pipe_count(newInputLine);
		if(num_pipes > 1){
			printf("Only two commands supported for pipe\n");
		}else if(num_pipes > 0){
			// commence with piping
			command_t** pipes;
			int pipesc;
			pipesc = make_pipes(newInputLine, &pipes);
			debug("pipesc = %d\n", pipesc);
			pipe_cmds(&pipes, pipesc);
		}else{
			process_line(newInputLine);
		}
    }
	return 0;
}

void process_file(const char* filepath){
	debug("processing file %s\n", filepath);
	size_t len = 80;
	char *line = (char *)malloc(80*sizeof(char)+1);
	FILE* file;
	debug("opening file\n");
	file = fopen(filepath, "r"); // open file
	if(file == NULL){
		printf("ERROR: file did not open\n");
		return;
	}
	while(getline(&line, &len, file) != -1){
		if(len > 80){
			printf("Error maximum line length exceeded\n");
			continue;
		}
		debug("line before: [%s]\n", line);
		int line_len;
		line_len = strlen(line);
		if(line[line_len-1] == '\n'){
			line[line_len - 1] = '\0'; // removing the new line
		}
		debug("line after: [%s]\n", line); 
		if(line != NULL){
			process_line(line);
		}
	}
	fclose(file);
}

int file_exists(const char *filepath){
	FILE *file;
	if((file = fopen(filepath, "r"))){
		fclose(file);
		return 1;
	}
	return 0;
}

void process_line(char *line){
	int comc;
	char **comv;
	debug("processing line by semicolons %s\n", line);
	comc = split_by_semicolon(line, &comv);
	int i;
	for(i = 0; i < comc; i++){
		int argc;
		char **argv;
		debug("processing command... makearg and removewhitespaces\n");
		argc = makearg(removeWhiteSpace(comv[i]), &argv);
		if(argc == -1){
			printf("ERROR: makearg function");
			continue;
		}
		process_command(&argv, argc);
	}
}

void process_command(char ***argvp, int argc){
	char **argv = *argvp;
	add_history_entry(&argv, argc); // add to history

	if(strcmp(argv[0], "exit") == 0){
		printf("\x1b[0m"); // reset color
		exit(0);
	}else if(strcmp(argv[0], "alias") == 0){
		if(argc < 2){
			print_alias_entry();
			return;
		}
		debug("processing alias command\n");
		char **aliasWords;
		debug("splitting by equals called\n");
		split_by_equals(argv[1], &aliasWords);
		debug(" - 0: %s\n", aliasWords[0]);
		debug(" - 1: %s\n", aliasWords[1]);
		debug("stripping quotes from: [%s]\n", aliasWords[1]);
		char* stripped_command;
		strip_quotes(aliasWords[1], &stripped_command);
		debug(" - stripped: [%s]\n", stripped_command);
		char **alias_argv;
		debug("calling makearg\n");
		int alias_argc = makearg(stripped_command, &alias_argv);
		debug("returned from makearg argc=%d\n", alias_argc);
		debug("calling add_alias_entry(%s, ...)\n", aliasWords[0]);
		add_alias_entry(aliasWords[0], &alias_argv, alias_argc);
		print_alias_entry();
	}else if(strcmp(argv[0], "unalias") == 0){
		debug("processing unalias command\n");
		remove_alias_entry(argv[1]);   
		print_alias_entry();
	}else if(strcmp(argv[0], "history") == 0){
		debug("processing history command\n");
		print_history_entry();
	}else if(strcmp(argv[0], "!!") == 0){
		debug("processing !! command\n");
		history_entry_t* prev_command;
		prev_command = last_command();
		if(prev_command != NULL){
			process_command(&prev_command->argv, prev_command->argc);
		}
	}else if(argv[0][0] == '!'){
		debug("processing !number command\n");
		int history_index;
		history_index = atoi(&argv[0][1]);
		history_entry_t* hist_command;
		hist_command = history_entry_lookup(history_index);
		if(hist_command != NULL){
			process_command(&hist_command->argv, hist_command->argc);
		}
	}else if(strcmp(argv[0], "export") == 0){
		if(argc < 2){
			printf("ERROR: printing out all current exported variables\n");
			print_var_entry();
			return;
		}
		char **exportWords;
		debug("splitting by equals called\n");
		split_by_equals(argv[1], &exportWords);
		char *strip_word;
		strip_quotes(exportWords[1], &strip_word);
		var_entry_t* var_entry = add_var_entry(exportWords[0], strip_word);
		if(strcmp(exportWords[0], "PATH") == 0){
			// set path
			setenv(var_entry->name, var_entry->value, 1);
		}
	}else if(strcmp(argv[0], "unset") == 0){
		if(argc < 2){
			printf("ERROR: printing out all current exported variables\n");
			print_var_entry();
			return;
		}
		remove_var_entry(argv[1]);
	}else{
		debug("executing\n");
		// check for alias
		alias_entry_t* alias_given;
		alias_given = alias_entry_lookup(argv[0]);
		if(alias_given != NULL){
			execute(alias_given->argv);
			return; 
		}else{
			execute(argv);
			return; 
		}
	}
}

// performs execvp on input given
void execute(char **argv){
    pid_t childpid;
    int status;
    // fork a child and execvp to bash compatible user input
    if((childpid = fork()) == -1){
        perror("Error in the fork");
        exit(1);
    }else if(childpid == 0){ /* child code */
        debug("calling execv\n");
        if (execvp(argv[0], argv) < 0) {
            perror("Exec failed");
            exit(1);
        }
		exit(0); // if it worked, exit successfully
    }else if(childpid != waitpid(childpid, &status, 0)){ /* parent code */
		// accept status from child (must specify child so it doesn't conflict with piping)
		printf("[%d] = status returned\n", status);
        printf("A signal occurred before the child exited");
    } 
	return;
}
