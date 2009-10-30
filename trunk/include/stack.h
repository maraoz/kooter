/* 
 * stack.h
 */

#ifndef _stack_h
#define _stack_h

typedef int T;

#define MAX_ELEM 100

typedef struct {
    int tail;
    T data[MAX_ELEM];
} stack_t;


/**
 * is_empty
 * devuelve TRUE si el stack est� vacia y FALSE si tiene elementos
 */
int is_empty(stack_t * s);

/**
 * is_full
 * devuelve TRUE si el stack est� llena y no acepta m�s elementos
 */
int is_full(stack_t * s);

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
int push(stack_t * s, T data);

/**
 * destack
 * Devuelve y elimina el primer elemento del stack.
 */
T pop(stack_t * s);

/**
 * peek
 * Devuelve sin eliminar el primer elemento del stack;
 */
T peek(stack_t * s);



/**
 * size
 * devuelve la cantidad delementos en el stack
 */
int size(stack_t * s);

#endif
