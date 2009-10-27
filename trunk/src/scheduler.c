
/* 
 * scheduler.c
 */

#include "../include/defs.h"
#include "../include/scheduler.h"
#include "../include/stdio.h"
#include "../include/queue.h"
#include "../include/util.h"
#include "../include/kc.h"

//contiene el pid del proceso actual
extern pid_t current_process;
// array de bloques de control de procesos
extern context_t bcp[MAX_PROCESSES];

//cola de pids de procesos en uso
queue_t * used_pids_q;

// cola de procesos en estado READY y su puntero (para manejo)
queue_t ready_processes;
queue_t * ready_processes_q;

// vector que almacena si el proceso esta bloqueado o no
boolean is_blocked_t[MAX_PROCESSES]={0};

// vector que almacena tiempo consumido por el proceso
unsigned long time_consumed[MAX_PROCESSES]={0};
unsigned long time_total = 1UL;

// scheduler cycles the current process worked in
int work_cycles = 1;

void context_switch(void) {
    __asm__("int $0x08");
}

int init_scheduler(void) {
    ready_processes_q = &ready_processes;
    queue_init(ready_processes_q);

    int i;
    for (i=0; i<MAX_PROCESSES; i++) {
        is_blocked_t[i] = FALSE;
        time_consumed[i] = 0;
    }

    return 0;
}

int block(int pid) {
    if (is_blocked_t[pid]) {
        //un proceso bloqueado no puede bloquearse nuevamente
        return -1;
    }
    is_blocked_t[pid] = TRUE;

    time_total -= time_consumed[pid];
    time_consumed[pid] = 0L;


    context_switch();
    return 0;
}

int unblock(int pid) {
    if (!is_blocked_t[pid]) {
        // si no est� bloqueado da error intentar desbloquearlo
        return -1;
    }
    is_blocked_t[pid] = FALSE;
    return enqueue(ready_processes_q, pid);
}

void magic_algorithm(void) {
    // roundrobin
    pid_t np = dequeue(ready_processes_q);
    if (current_process != np) {
        //TODO: ojo, ver si va o no
//         flush();
        current_process = np;
    }
}

void magic_algorithm2(void) {
    // weighted priorities
    if (work_cycles < bcp[current_process].process.priority) {
        work_cycles++;
        return;
    }
    work_cycles = 1;
    pid_t np = dequeue(ready_processes_q);
    if (current_process != np) {
        flush();
        current_process = np;
    }
}

void run_next_process(void) {
    magic_algorithm();
}

void scheduler(void){

    if (!is_blocked(current_process)) {
        desalojate(current_process);
    }

    magic_algorithm2();

    time_consumed[current_process] += 1UL;
    time_total += 1UL;
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



double use_percentage(pid_t pid) {
    return time_consumed[pid] / ((double) time_total);
}


char number_str[13];

void print_process_use_percentage(pid_t pid) {

    int perc = use_percentage(pid) * 100;

    itoa(pid, number_str);
    puts(number_str);
    puts (" ----------> ");

    itoa(perc, number_str);
    puts(number_str);
    puts(" % ");
    put_char('\n');


}

void top(void) {
    putln("--------/ TOPAZ /---------");
    putln("  id     Use Percentage   ");
    queue_t * q = used_pids_q;
    T curr, first = dequeue(q);
    if (first == -1) {
        // no processes (WTF?)
        putln("           WTF            ");
        return;
    }
    print_process_use_percentage(first);
    enqueue(q, first);
    while (first != (curr = peek(q))) {
        dequeue(q);
        print_process_use_percentage(curr);
        enqueue(q, curr);
    }
    // finished cycling
    putln("--------------------------");
    return;
}