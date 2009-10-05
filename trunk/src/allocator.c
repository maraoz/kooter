
#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/kc.h"

extern void * eokl;
char * mem;

void allocator_init() {
    char * mem = (char*)eokl + 4096;
}

void * malloc(int size){
    return mem+=size;
}

void free(void * mem){
}
