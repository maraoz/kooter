/* 
 * queue.c
 */


void queue_init(queue_t * q) {
    q->head = 0;
    q->tail = 0;
}

int is_empty(queue_t * q){
    return q->head == q->tail;
}

int is_full(queue_t * q) {
    return q->tail == q->head - 1;
}

int enqueue(queue_t * q, T data) {
    if (is_full(q))
        return -1;
    q->tail = (q->tail + 1) % MAX_ELEM;
    q->data[q->tail] = data;
    return 0;
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