/* 
 * queue.c
 * ADT para colas
 */

#include <stdio.h>

#include "../include/queue.h"


void queue_init(queue_t * q) {
    q->head = 0;
    q->tail = 0;
}

int is_empty(queue_t * q){
    return q->head == q->tail;
}

int is_full(queue_t * q) {
    return ((q->tail + 1) % MAX_ELEM) == q->head;
}

int enqueue(queue_t * q, T elem) {
    if (is_full(q)) {
        return -1;
    }
    q->data[q->tail] = elem;
    q->tail = (q->tail + 1) % MAX_ELEM;
    return 0;
}

T dequeue_element(queue_t * q, T elem) {
    T curr, first = dequeue(q);
    if (first == -1) {
        // empty queue
        return -1;
    }
    if (first == elem) {
        // found it on first element in queue
        return elem;
    }
    enqueue(q, first);
    while (first != (curr = peek(q))) {
        dequeue(q);

        if (curr != elem) {
            enqueue(q, curr);
        } else {
            return elem;
        }
    }
    // not found
    return -1;
}

T dequeue(queue_t * q) {
    if (is_empty(q)) {
        return -1;
    }
    T ret_val;
    ret_val = q->data[q->head];
    q->head = (q->head + 1) % MAX_ELEM;
    return ret_val;
}

T peek(queue_t * q) {
    if (is_empty(q)) {
        return -1;
    }
    return q->data[q->head];
}

T peek_last(queue_t * q) {
    if (is_empty(q)) {
        return -1;
    }
    int prev_index = ((q->tail - 1)<0)?MAX_ELEM-1:q->tail -1;
    return q->data[prev_index];
}

int size(queue_t * q) {
    return (q->head > q->tail)? MAX_ELEM - (q->tail - q->head) : q->tail - q->head;
}

