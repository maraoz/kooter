#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/kc.h"
#include "../include/allocator.h"

extern void * eokl;
char * mem;

void allocator_init() {
    mem = (char*)eokl + 4096;
}

void * malloc(int size){
    return mem+=size;
}

void free(void * mem){
}

PAGE * palloc(int size){
    return malloc(4096*size);
}

void down_p(PAGE * page){
    return;
}

void up_p(PAGE * page){
    return;
}