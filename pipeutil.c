// [Assign3-cs240-msh-pipeutil.c]

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "argparse.h"
#include "pipeutil.h"
#include "history.h"

//#define DEBUG_ENABLE
#include "debug.h"

#define READ_END 0
#define WRITE_END 1

// This function will call the function to pipe the commands in "pipesp" if "pipesc" is 2 (only supports piping between 2 commands) 
void pipe_cmds(command_t*** pipesp, int pipesc){
    command_t ** pipes;
    pipes = *pipesp;
    int i;
    for(i = 0; i < pipesc; i++){
        add_history_entry(&pipes[i]->argv, pipes[i]->argc);
    }
    if (pipesc != 2) {
        printf("ERROR: Only 2 commands supported for piping\n");
        return;
    }
    pipe2_execvp(pipes[0]->argv, pipes[1]->argv);
}

// This functoin uses execvp, pipe, fork, and dup2 to do piping given two command vectors. (only supports 2 commands with 1 arg each)
void pipe2_execvp(char **cmd0_argv, char** cmd1_argv){
    int fd[2];
    int pipe_return;
    pid_t pid;
    int status;

    pipe_return = pipe(fd); // pipe fd (now we get read end and write end)
    if(pipe_return == -1){
        printf("ERROR: pipe\n");
        return;
    }
    // create 2 processes with fd[2] already piped
    pid = fork(); 
    if(pid == -1){
        printf("ERROR: fork\n");
        return;
    }
    if(pid==0){ // child process
        // close the read end because this will execvp a command for parent, and read input as normal
        close(fd[READ_END]); 
        // redirect output to fd[WRITE_END] instead of stdout
        dup2(fd[WRITE_END], STDOUT_FILENO);
        close(fd[WRITE_END]);
        // execute first command with argument and write output to write end instead of stdout
        execvp(cmd0_argv[0], cmd0_argv); 
        fprintf(stderr, "Failed to execute '%s'\n", cmd0_argv[0]);
        exit(1); // unsuccessful exit
    }else{ // parent process
        pid=fork();
        if(pid == -1){
            printf("ERROR: fork\n");
            return;
        }
        if(pid==0){ // other child of parent process
            // close write end, write noramlly to stdout etc. 
            close(fd[WRITE_END]);  
            // take input from fd[READ_END] instead of stdin
            dup2(fd[READ_END], STDIN_FILENO); 
            close(fd[READ_END]); 
            // executes second command reading input from first command
            execvp(cmd1_argv[0], cmd1_argv); 
            fprintf(stderr, "Failed to execute '%s'\n", cmd1_argv[1]);
            exit(1); // unsuccessful exit
        }else{ // close both ends of the pipe for parent
            int stat;
            close(fd[READ_END]);
            close(fd[WRITE_END]);
            waitpid(pid, &stat, 0); // wait for exit status from child
        }
        waitpid(pid, &status, 0);
    }
}

// This function returns the number of commands involved in piping and allocates space for an array of commands to be piped
int make_pipes(char* line, command_t*** pipevp){
    int i, j;
    char **pipev;
    int pipec;
    pipec = split_by_pipe(line, &pipev);
    debug("pipec: %d\n", pipec);
    for(i = 0; i < pipec; i++){
        debug("Pipe split [%d]: %s\n", i, pipev[i]);
    }
    
    command_t **pipes = (command_t**)malloc(pipec*sizeof(command_t*));
    if(pipes == NULL){
        printf("ERROR: allocating command_t pipes**\n");
        return -1;
    }
    for(i = 0; i < pipec; i++){
        pipes[i] = (command_t*)malloc(sizeof(command_t));
        pipes[i]->argv = NULL;
        pipes[i]->argc = 0;
    }

    // make arguments from commands
    int argc;
    char** argv;
    for(i = 0; i < pipec; i++){
        debug("COMMAND %d: \n", i);
        argc = makearg(removeWhiteSpace(pipev[i]), &argv);
        pipes[i]->argv = argv;
        pipes[i]->argc = argc;
        debug("Argc = %d\n", argc);
        for(j = 0; j < argc; j++){
            debug("Arg[%d] = %s\n", j, argv[j]);
        }
    }

    for(i = 0; i < pipec; i++){
        debug("pipe command[%d]\n", i);
        for(j = 0; j < pipes[i]->argc; j++){
            debug("argv[%d] = [%s]\n", j, pipes[i]->argv[j]);
        }
    }
    *pipevp = pipes;
    return pipec;
}

// return the number of '|' found in a string
int pipe_count(char *line){
    int pipe_count = 0;
    int i = 0;
    while(line[i] != '\0'){
        if(line[i] == '|'){
            pipe_count++;
        }
        i++;
    }
    return pipe_count;
}
