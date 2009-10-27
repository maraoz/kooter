#ifndef _shell_h
#define _shell_h

#define HIST_LEN 10
#include "./defs.h" 

int switch_tty(int new_tty);

void print_nline();

int str_cmp(char *s, char *t);

int strncpy(char *s, char *t, int n);

void separaPorEspacios(char *s, char out[][LONG_STR_TKN]);

int llamaFunc(char s[][LONG_STR_TKN]);

void shell();

int atoi(char *s);

int str_cpy(char *s, char *t);

int str_len(char *s);

#endif
