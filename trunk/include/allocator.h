
/*
 * allocator.h
 */

#ifndef _allocator_h
#define _allocator_h

#include "./defs.h"

void init_pagination();

PAGE * palloc(int cant);

void pfree(PAGE * page, int cant);

void * malloc(int size);

void free(void * mem);

void down_p(PAGE * page, int cant);

void up_p(PAGE * page, int cant);

#endif
