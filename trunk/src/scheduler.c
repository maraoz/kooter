
/* 
 * scheduler.c
 */

#include "../include/defs.h"
#include "../include/scheduler.h"
#include "../include/queue.h"


queue_t * ready_processes_q;
queue_t * blocked_processes_q;


void block(int pid);

void unblock(int pid);

int pid next_process(void){
    return dequeue(ready_processes_q);
}

int pid create_process(void);

void desalojate(int pid);

