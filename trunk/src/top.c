#include "../include/defs.h"
#include "../include/scheduler.h"
#include "../include/stdio.h"
#include "../include/queue.h"
#include "../include/util.h"
#include "../include/kc.h"
#include "../include/keyboard.h"
#include "../include/process.h"


// ttys
extern TTY tty[8];

//cola de pids de procesos en uso
extern queue_t * used_pids_q;


char number_str[13];
char view[8][25][80][2];
void print_process_use_percentage(pid_t pid, int line) {
    int currentTTY = get_current_tty();

    line = line + 3;

    itoa(pid, number_str);
    int j;
    for (j = 0; j<20; j++) {
        view[currentTTY][line][j+5][0] = number_str[j];
        if (!number_str[j+1]) break;
    }
    char * name = getProcessName(pid);
    for (j = 0; j<20; j++) {
        view[currentTTY][line][j+10][0] = name[j];
        view[currentTTY][line][j+10][1] = DARK_BLUE_TXT;
        if (!name[j+1]) break;
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

    char * state = getProcessState( pid);


    for (j = 0; j<20; j++) {
        view[currentTTY][line][j+60][0] = state[j];
        switch(state[0]) {
        case 'W':
            view[currentTTY][line][j+60][1] = YELLOW_TXT;
            break;
        case 'B':
            view[currentTTY][line][j+60][1] = RED_TXT;
            break;
        default:
            view[currentTTY][line][j+60][1] = GREEN_TXT;
        }

        view[currentTTY][line][j+61][0] = ' ';
        if (!state[j+1]) break;
    }

}

void init_view(){
    int currentTTY = get_current_tty();
    int i,j;
    for (i=0; i<25;i++) {
        for (j=0; j<80;j++) {
            view[currentTTY][i][j][0] = ' ';
            view[currentTTY][i][j][1] = GRAY_TXT;
        }
    }
}
void showTop(void) {
    int currentTTY = get_current_tty();
    _Cli();
    move_cursor_to(0);
    check_offset('t', 4000);
    write(PANTALLA_FD, view[currentTTY], 4000);
    move_cursor_to(0);
    _Sti();
}


void top(void) {
    int currentTTY = get_current_tty();

    while(1) {
        init_view();

        char * header = "------------------------------------/ TOPAZ /-----------------------------------";
        char * heade2 = "|    PID      Name                Use Percentage               State           |";
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
            view[currentTTY][1][j][1] = (j!=0 && j!= 79)?WHITE_TXT:GRAY_TXT;
            view[currentTTY][2][j][0] = footer[j];
            view[currentTTY][24][j][0] = footer[j];
        }
        int i=0;
        int N_PROC = 20;


        queue_t * q = used_pids_q;

        int last = peek_last(q);

        _Cli();

            if (last == -1) {
                _Sti();
                return;
            }
            int np;
            do {
                np = dequeue(q);

                if (i++ < N_PROC)
                    print_process_use_percentage(np,i);
                enqueue(q, np);
                if (np == last) {
                    break;
                }
            } while (1);
        _Sti();

        showTop();
        int c = non_blocking_next_char();
        if (c != -1) {
            init_view();
            showTop();
            return;
        }
    }
    return;
}
