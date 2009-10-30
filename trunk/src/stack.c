/* 
 * stack.c
 * ADT para stacks
 */


#include "../include/stack.h"


void stack_init(stack_t * s) {
    s->tail = 0;
}

int stack_empty(stack_t * s){
    return s->tail == 0;
}

int stack_full(stack_t * s) {
    return s->tail == MAX_ELEM;
}

int push(stack_t * s, T elem) {
    if (stack_full(s)) {
        return -1;
    }
    s->data[s->tail] = elem;
    s->tail = s->tail + 1;
    return 0;
}

T pop(stack_t * s) {
    if (stack_empty(s)) {
        return -1;
    }
    s->tail--;
    return s->data[s->tail];

}

int stack_size(stack_t * s) {
    return  s->tail;
}

int _main(void) {
    stack_t my_stack;
    stack_t * s = &my_stack; // este se usa para operar
    
    stack_init(s);
    
    push(s, 5);
    push(s, 3);
    push(s, 1);
    push(s, 7);
    while (!stack_empty(s))
//         printf("%d\n", pop(s));
        ;
    
}
