
/*
 * allocator.h
 */

#ifndef _allocator_h
#define _allocator_h
PAGE * palloc(int n);

void pfree(PAGE * page);

void * malloc(int size);

void free(void * mem);

#endif