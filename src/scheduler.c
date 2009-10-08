
/* 
 * scheduler.c
 */

#include "../include/defs.h"
#include "../include/scheduler.h"
#include "../include/queue.h"
#include "../include/kc.h"

// cola de procesos en estado READY y su puntero (para manejo)
queue_t ready_processes;
queue_t * ready_processes_q;

// vector que almacena si el proceso esta bloqueado o no
boolean is_blocked_t[MAX_PROCESSES];

void context_switch(void) {
    __asm__("int $08");
}

int init_scheduler(void) {
    ready_processes_q = &ready_processes;
    queue_init(ready_processes_q);

    int i;
    for (i=0; i<MAX_PROCESSES; i++) {
        is_blocked_t[i] = FALSE;
    }

    return 0;
}

int block(int pid) {
    if (is_blocked_t[pid]) {
        //un proceso bloqueado no puede bloquearse nuevamente
        return -1;
    }
    is_blocked_t[pid] = TRUE;
    context_switch();
    return 0;
}

int unblock(int pid) {
    if (!is_blocked_t[pid]) {
        // si no está bloqueado da error intentar desbloquearlo
        return -1;
    }
    is_blocked_t[pid] = FALSE;
    return enqueue(ready_processes_q, pid);
}

void scheduler(void){

    pid_t np = dequeue(ready_processes_q);
    current_process = np;

    return;
}


boolean is_blocked(pid_t pid) {
    return is_blocked_t[pid];
}

int desalojate(int pid) {
    if (enqueue(ready_processes_q, pid) == -1) {
        // no hay lugar en la cola
        return -1;
    }
    return 0;
}

