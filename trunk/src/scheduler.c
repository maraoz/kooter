
/* 
 * scheduler.c
 */

#include "../include/defs.h"
#include "../include/scheduler.h"
#include "../include/stdio.h"
#include "../include/queue.h"
#include "../include/util.h"
#include "../include/kc.h"
#include "../include/keyboard.h"
#include <math.h>

#define not !



//contiene el pid del proceso actual
extern pid_t current_process;
// array de bloques de control de procesos
extern context_t bcp[MAX_PROCESSES];



// cola de procesos en estado READY y su puntero (para manejo)
queue_t ready_processes;
queue_t * ready_processes_q;

// vector que almacena si el proceso esta bloqueado o no
boolean is_blocked_t[CHANNEL_AMMOUNT][MAX_PROCESSES]={0};

// vector que almacena tiempo consumido por el proceso
unsigned long time_consumed[MAX_PROCESSES]={0};
unsigned long time_total = 1UL;

// scheduler cycles the current process worked in
int work_cycles = 1;


#define POWER_MAX 100
int powerBar[MAX_PROCESSES];




void context_switch(void) {
    __asm__("int $0x08");
}

int init_scheduler(void) {
    ready_processes_q = &ready_processes;
    queue_init(ready_processes_q);

    int i,channel;
    for (i = 0; i<MAX_PROCESSES; i++) {
        for (channel = 0; channel<CHANNEL_AMMOUNT; channel++) {
            is_blocked_t[channel][i] = FALSE;
        }
        time_consumed[i] = 0;
        powerBar[i] = 0;
    }

    return 0;
}

int block(int pid, int channel) {
    if (is_blocked_t[channel][pid]) {
        //un proceso bloqueado no puede bloquearse nuevamente
        return -1;
    }
    bcp[pid].process.isBlocked = TRUE;
    is_blocked_t[channel][pid] = TRUE;

    time_total -= time_consumed[pid];
    time_consumed[pid] = 0L;

    context_switch();
    return 0;
}

int unblock(int pid, int channel) {
    if (!is_blocked_t[channel][pid]) {
        // si no estï¿½ bloqueado da error intentar desbloquearlo
        return -1;
    }
    bcp[pid].process.isBlocked = FALSE;
    is_blocked_t[channel][pid] = FALSE;
    if (bcp[pid].process.isAlive == FALSE)
        return -1;
    return enqueue(ready_processes_q, pid);
}

void magic_algorithm1(void) {
    // roundrobin
    pid_t np = dequeue(ready_processes_q);
    if (np == -1) {
        unblock(0, CNL_FOREVER);
        np = 0;
    }
    current_process = np;
}

void magic_algorithm2(void) {
    // bingo
    int np;
    do {
        np = dequeue(ready_processes_q);
        if (np == -1) {
            unblock(0, CNL_FOREVER);
            np = 0;
        }
        int priority = bcp[np].process.priority;
        int tickets = 18 - priority * priority;
        unsigned int r = rand_int(20);

        if (r < tickets) {
            break;
        }
        enqueue(ready_processes_q, np);
    } while (1);
    current_process = np;
}

void magic_algorithm3(void) {
    // power bars
    int np;
    do {
        np = dequeue(ready_processes_q);
        if (np == -1) {
            unblock(0, CNL_FOREVER);
            np = 0;
        }
        int priority = bcp[np].process.priority;
        powerBar[np] += 16 - priority * priority;
        if (powerBar[np] >= 20 ) {
            powerBar[np] = powerBar[np] % 20;
            break;
        }
        enqueue(ready_processes_q, np);
    } while (1);
    current_process = np;

}



void run_next_process(void) {
    // Cambiar acá si se quiere cambiar
    // el algoritmo de scheduling
    magic_algorithm2();
}

void scheduler(void){

    if (bcp[current_process].process.isAlive &&
            ! is_blocked(current_process, CNL_ANY)) {
        desalojate(current_process);
    }

    run_next_process();

    time_consumed[current_process] += 1UL;
    time_total = (time_total % 100) + 1UL;
    if (time_total % 100 == 0) {
        int i;
        for(i = 0; i<MAX_PROCESSES; i++)
            time_consumed[i] = 0;
    }

    return;
}



boolean is_blocked_in_any(pid_t pid) {
    return bcp[pid].process.isBlocked;
}

boolean is_blocked(pid_t pid, int channel) {
    if (channel == CNL_ANY)
        return is_blocked_in_any(pid);
    return is_blocked_t[channel][pid];
}

int desalojate(int pid) {
    if (bcp[pid].process.isAlive) {
        if (enqueue(ready_processes_q, pid) == -1) {
            // no hay lugar en la cola
            return -1;
        }
        // el proceso va a la cola de readies
        return 0;
    } else {
        // desaparece el proceso muerto
//         wake_up_father((bcp[pid].dad_pid));
        return 0;
    }
    
}



int use_percentage(pid_t pid) {
    return (100*time_consumed[pid]) / time_total;
}





