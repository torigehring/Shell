// pipeutil.h

#ifndef PIPEUTIL_H
#define PIPEUTIL_H

typedef struct command { 
    char **argv; 
    int argc;
} command_t; 

int make_pipes(char* line, command_t*** pipevp);
void pipe_cmds(command_t*** pipesp, int pipesc);
void pipe2_execvp(char **cmd0_argv, char** cmd1_argv);
int pipe_count(char *line);

#endif