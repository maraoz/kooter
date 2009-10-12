/* 
 * queue.h
 */

#ifndef _queue_h
#define _queue_h

typedef int T;

#define MAX_ELEM 10

typedef struct {
    int head;
    int tail;
    T data[MAX_ELEM];
} queue_t;


/**
 * is_empty
 * devuelve TRUE si la cola est� vacia y FALSE si tiene elementos
 */
int is_empty(queue_t * q);

/**
 * is_full
 * devuelve TRUE si la cola est� llena y no acepta m�s elementos
 */
int is_full(queue_t * q);

/**
 * queue_init
 * inicializa la cola
 */

void queue_init(queue_t * q);

/**
 * enqueue
 * Agrega el dato "data" al final de la cola
 * Devuelve -1 si no se pudo realizar la operaci�n
 * o 0 si no hubo problema.
 */
int enqueue(queue_t * q, T data);

/**
 * dequeue
 * Devuelve y elimina el primer elemento de la cola.
 */
T dequeue(queue_t * q);

/**
 * dequeue
 * Devuelve sin eliminar el primer elemento de la cola;
 */
T peek(queue_t * q);

#endif
