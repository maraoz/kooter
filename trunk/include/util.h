#ifndef _util_h_
#define _util_h_

int isalpha(int c);

int isdigit(int c);

int isalphanum(int c);

int abs(int a);

int min(int a, int b);

int max(int a, int b);

void wait(int t);

void itoa(int num, char v[]);

void check_offset(char label, int count);

#endif