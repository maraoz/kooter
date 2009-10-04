
/* 
 * scheduler.c
 */

#include "../include/defs.h"
#include "../include/scheduler.h"
#include "../include/queue.h"

queue_t ready_processes;
queue_t blocked_processes;
queue_t * ready_processes_q;
queue_t * blocked_processes_q;


int init_scheduler(void) {
    ready_processes_q = &ready_processes;
    blocked_processes_q = &blocked_processes;
    q_init(ready_processes_q);
    q_init(blocked_processes_q);
}

int block(int pid) {
    if (enqueue(blocked_processes_q, pid) != -1) {
        return 0;
    }
    return -1;
}

void unblock(int pid) {
    /* TODO: consultar como hacer esto con los chicos
     * porque ¿siempre se despierta sabiendo el pid?
     * en tal caso no es conveniente una cola
     */
    return -1
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

