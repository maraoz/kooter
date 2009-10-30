#ifndef _util_h_
#define _util_h_

int isalpha(int c);

int isdigit(int c);

int isalphanum(int c);

int abs(int a);

int min(int a, int b);

int max(int a, int b);

void wait(int t);

void habilitar_int_mouse (void);

void deshabilitar_int_mouse (void);

void itoa(int num, char v[]);

void uitoa (unsigned int n, char ret[]);

void check_offset(char label, int count);

void panic(char label, int n);

unsigned int get_random();

unsigned int rand_int(unsigned int not_inclusive_max);

void *memcpy(void *dest, const void *src, size_t count);

void* memset(void* s, int c, size_t n); 

#endif
