#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "argparse.h"
#include "pipeutil.h"

int main(){
    int num_pipes = pipe_count(" ls | wc ");
    printf("num_pipes = %d\n", num_pipes);
    int i;
    command_t** pipes;
    int pipesc;
    pipesc = make_pipes("ls -l | grep pipe", &pipes);
    printf("pipesc = %d\n", pipesc);
    pipe_cmds(&pipes, pipesc);
    return 0;
}