# Victoria Gehring CS240 Assign 3
# // [Assign3-cs240-msh-Makefile]
# To build msh type "make msh"
CFLAGS=-Wall -O

testhistory: history.o testhistory.o argparse.o
	gcc ${CFLAGS} history.o testhistory.o argparse.o -o testhistory

history.o: history.c
	gcc ${CFLAGS} -c history.c

testhistory.o: testhistory.c
	gcc ${CFLAGS} -c testhistory.c

argparse.o: argparse.c
	gcc ${CFLAGS} -c argparse.c

testargparse.o: testargparse.c
	gcc ${CFLAGS} -c testargparse.c

testargparse: argparse.o testargparse.o
	gcc ${CFLAGS} argparse.o testargparse.o -o testargparse

testalias: alias.o testalias.o argparse.o debug.h
	gcc ${CFLAGS} alias.o testalias.o argparse.o -o testalias

alias.o: alias.c
	gcc ${CFLAGS} -c alias.c

testalias.o: testalias.c
	gcc ${CFLAGS} -c testalias.c

msh: msh.o alias.o history.o argparse.o varparse.o pipeutil.o
	gcc ${CFLAGS} msh.o alias.o history.o argparse.o varparse.o pipeutil.o -o msh

msh.o: msh.c
	gcc ${CFLAGS} -c msh.c

varparse.o: varparse.c
	gcc ${CFLAGS} -c varparse.c

testvarparse: varparse.o testvarparse.o
	gcc ${CFLAGS} varparse.o testvarparse.o -o testvarparse

testvarparse.o: testvarparse.c
	gcc ${CFLAGS} -c testvarparse.c

pipeutil.o: pipeutil.c argparse.c
	gcc ${CFLAGS} -c pipeutil.c argparse.c

testpipeutil: pipeutil.o argparse.o
	gcc ${CFLAGS} pipeutil.o argparse.o

tarball: msh.c argparse.c argparse.h testargparse.c varparse.c varparse.h testvarparse.c pipeutil.c pipeutil.h testpipeutil.c history.c history.h testhistory.c alias.c alias.h testalias.c debug.h Makefile README.md
	tar -cvf  CS240-Ass3.tar msh.c argparse.c argparse.h testargparse.c varparse.c varparse.h testvarparse.c pipeutil.c pipeutil.h testpipeutil.c history.c history.h testhistory.c alias.c alias.h testalias.c debug.h Makefile README.md