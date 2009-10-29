
/*
 * scheduler.h
 */

#include "../include/defs.h"

#ifndef _scheduler_h
#define _scheduler_h

/**
 * init_scheduler
 * inicializa estructuras y tablas internas del scheduler
 * retorna -1 en caso de error
 */
int init_scheduler(void);

/**
 * block
 * pone al proceso en estado de bloqueado (esperando a ser despertado)
 */
int block(int pid, int channel);

#define block_me(channel) block(current_process, channel)
#define wait_children() block_me(CNL_CHILDREN)


/**
 * unblock
 * despierta al proceso y lo pone en la lista de ready
 * devuelve -1 en caso de error
 */
int unblock(int pid, int channel);

#define wake_up_father(pid) unblock(pid, CNL_CHILDREN)

/**
 * run_next_process
 * saca un nuevo proceso de la cola de ready y lo pone a correr
 * (sin resguardar el viejo, eso queda bajo responsabilidad
 * de otro)
 */
void run_next_process(void);

/**
 * scheduler
 * cambia el proceso actual si es necesario
 * (segun el algoritmo del scheduler implementado)
 */
void scheduler(void);

/**
 * is_blocked
 * devuelve true si el proceso est� bloqueado o false en caso contrario
 */
boolean is_blocked(pid_t pid, int channel);

/**
 * desalojate
 * vuelve a poner el proceso en la cola de readys
 */
int desalojate(int pid);


/**
 * top
 * display on terminal processes use of processor
 */

void top(void);

#endif


