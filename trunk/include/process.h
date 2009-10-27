#ifndef _process_h
#define _process_h

void process_creator();

int init_pids(void);

int get_current_tty(void);

process_t create_process(int (*funcion)(), int pages_qty, int argc, char **argv, int gid, int priority, int background, int tty, pid_t dad_pid, char * name);
#endif
