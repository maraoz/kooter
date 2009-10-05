
/* 
 * scheduler.c
 */

#include "../include/defs.h"
#include "../include/scheduler.h"
#include "../include/queue.h"

// cola de procesos en estado READY y su puntero (para manejo)
queue_t ready_processes;
queue_t * ready_processes_q;

// vector que almacena si el proceso esta bloqueado o no
boolean is_blocked[MAX_PROCESSES];


int init_scheduler(void) {
    ready_processes_q = &ready_processes;
    q_init(ready_processes_q);

    int i;
    for (i=0; i<MAX_PROCESSES; i++) {
        is_blocked[i] = FALSE;
    }

    return 0;
}

int block(int pid) {
    if (is_blocked[pid]) {
        //un proceso bloqueado no puede bloquearse nuevamente
        return -1;
    }
    is_blocked[pid] = TRUE;
    return 0;
}

void unblock(int pid) {
    is_blocked[pid] = FALSE;
    return enqueue(ready_processes_q, pid);
}

int pid next_process(void){
    return dequeue(ready_processes_q);
}


int desalojate(int pid) {
    if (enqueue(ready_processes_q) == -1) {
        // no hay lugar en la cola
        return -1
    }
    return 0;
}

