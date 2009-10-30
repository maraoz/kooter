/* 
 * stack.h
 */

#ifndef _stack_h
#define _stack_h

typedef int S;

#define MAX_ELEM 100

typedef struct {
    int tail;
    S data[MAX_ELEM];
} stack_t;


/**
 * is_empty
 * devuelve TRUE si el stack est� vacia y FALSE si tiene elementos
 */
int stack_empty(stack_t * s);

/**
 * is_full
 * devuelve TRUE si el stack est� llena y no acepta m�s elementos
 */
int stack_full(stack_t * s);

/**
 * stack_init
 * inicializa el stack
 */

void stack_init(stack_t * s);

/**
 * enstack
 * Agrega el dato "data" al final del stack
 * Devuelve -1 si no se pudo realizar la operaci�n
 * o 0 si no hubo problema.
 */
int push(stack_t * s, S data);

/**
 * destack
 * Devuelve y elimina el primer elemento del stack.
 */
S pop(stack_t * s);




/**
 * size
 * devuelve la cantidad delementos en el stack
 */
int stack_size(stack_t * s);

#endif
