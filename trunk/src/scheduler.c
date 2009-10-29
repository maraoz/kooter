
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

// ttys
extern TTY tty[8];
extern int focusedTTY; 

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
boolean is_blocked_t[CHANNEL_AMMOUNT][MAX_PROCESSES]={0};

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

    int i,channel;
    for (i = 0; i<MAX_PROCESSES; i++) {
        for (channel = 0; channel<CHANNEL_AMMOUNT; channel++) {
            is_blocked_t[channel][i] = FALSE;
        }
        time_consumed[i] = 0;
    }

    return 0;
}

int block(int pid, int channel) {
    if (is_blocked_t[channel][pid]) {
        //un proceso bloqueado no puede bloquearse nuevamente
        return -1;
    }
    is_blocked_t[channel][pid] = TRUE;

    time_total -= time_consumed[pid];
    time_consumed[pid] = 0L;

    context_switch();
    return 0;
}

int unblock(int pid, int channel) {
    if (!is_blocked_t[channel][pid]) {
        // si no est� bloqueado da error intentar desbloquearlo
        return -1;
    }
    is_blocked_t[channel][pid] = FALSE;
    return enqueue(ready_processes_q, pid);
}

void magic_algorithm(void) {
    // roundrobin
    pid_t np = dequeue(ready_processes_q);
    if (np == -1) {
        unblock(0, CNL_FOREVER);
        np = 0;
    }
    if (current_process != np) {
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
    if (np == -1) {
        unblock(0, CNL_FOREVER);
        np = 0;
    }
    current_process = np;
}

void run_next_process(void) {
    magic_algorithm();
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
    int channel;
    for (channel = 0; channel<CHANNEL_AMMOUNT; channel++) {
        if (is_blocked_t[channel][pid])
            return TRUE;
    }
    return FALSE;
}

boolean is_blocked(pid_t pid, int channel) {
    if (channel == CNL_ANY)
        return is_blocked_in_any(pid);
    return is_blocked_t[channel][pid];
}

int desalojate(int pid) {
    if (enqueue(ready_processes_q, pid) == -1) {
        // no hay lugar en la cola
        return -1;
    }
    return 0;
}







char number_str[13];
char view[8][25][80][2];

int use_percentage(pid_t pid) {
    return (100*time_consumed[pid]) / time_total;
}

void print_process_use_percentage(pid_t pid, int line) {
    int currentTTY = get_current_tty();

    line = line + 3;

    itoa(pid, number_str);
    int j;
    for (j = 0; j<20; j++) {
        view[currentTTY][line][j+3][0] = number_str[j];
        if (!number_str[j+1]) break;
    }
    char * flechita = " ----------> ";
    for (j = 0; j<20; j++) {
        view[currentTTY][line][j+20][0] = flechita[j];
        if (!flechita[j+1]) break;
    }


    int perc = use_percentage(pid);

    itoa(perc, number_str);

    for (j = 0; j<20; j++) {
        view[currentTTY][line][j+40][0] = number_str[j];
        view[currentTTY][line][j+41][0] = ' ';
        if (!number_str[j+1]) break;
    }

    char * porce = "% ";
    for (j = 0; j<20; j++) {
        view[currentTTY][line][j+43][0] = porce[j];
        if (!porce[j+1]) break;
    }

    for (j = 0; j<20; j++) {
        view[currentTTY][line][j+60][0] = bcp[pid].process.name[j];
        if (!bcp[pid].process.name[j+1]) break;
    }

}

void init_view(){
    int currentTTY = get_current_tty();
    int i,j;
    for (i=0; i<25;i++) {
        for (j=0; j<80;j++) {
            view[currentTTY][i][j][0] = ' ';
            view[currentTTY][i][j][1] = WHITE_TXT;
        }
    }
}
void showTop(void) {
    int currentTTY = get_current_tty();
    tty[currentTTY].cursor = 0;
    check_offset('t', 4000);
    write(PANTALLA_FD, view[currentTTY], 4000);
    tty[currentTTY].cursor = 0;
}

void top(void) {
    int currentTTY = get_current_tty();

    while(1) {
        init_view();
        _Cli();

        char * header = "------------------------------------/ TOPAZ /-----------------------------------";
        char * heade2 = "|                         id     Use Percentage    name                        |";
        char * footer = "--------------------------------------------------------------------------------";
        char * column = "|||||||||||||||||||||||||";
        int j;
        for (j = 0; j<25; j++) {
            view[currentTTY][j][0][0] = column[j];
            view[currentTTY][j][79][0] = column[j];
        }
        for (j = 0; j<80; j++) {
            view[currentTTY][0][j][0] = header[j];
            view[currentTTY][1][j][0] = heade2[j];
            view[currentTTY][2][j][0] = footer[j];
            view[currentTTY][24][j][0] = footer[j];
        }
        int i=0;
        int N_PROC = 20;


        queue_t * q = used_pids_q;
        T curr;
        T first = dequeue(q);
        if (first == -1) return;
        print_process_use_percentage(first, ++i);

        enqueue(q, first);
        while (first != (curr = peek(q))) {
            dequeue(q);
            if (i++ < N_PROC)
                print_process_use_percentage(curr,i);
            enqueue(q, curr);
        }

        showTop();
        int c = non_blocking_next_char();
        if (c != -1) {
            init_view();
            showTop();
            return;
        }
        _Sti();
    }
    return;
}