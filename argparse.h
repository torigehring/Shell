#ifndef ARGPARSE_H
#define ARGPARSE_H

int makearg(char *s, char ***args);
int numArgs(char *s);
char * removeWhiteSpace(char *s);
int split_by_semicolon(char *s, char ***args);
int split_by_equals(char *s, char ***args);
void strip_quotes(char *s, char **newS);
int split_by_pipe(char *s, char ***args);

#endif