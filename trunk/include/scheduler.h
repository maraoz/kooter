
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
int block(int pid);

/**
 * unblock
 * despierta al proceso y lo pone en la lista de ready
 * devuelve -1 en caso de error
 */
int unblock(int pid);

/**
 * scheduler
 * cambia el proceso actual si es necesario
 * (segun el algoritmo del scheduler implementado)
 */
void scheduler(void);

/**
 * is_blocked
 * devuelve true si el proceso está bloqueado o false en caso contrario
 */
boolean is_blocked(pid_t pid);

/**
 * desalojate
 * vuelve a poner el proceso en la cola de readys
 */
int desalojate(int pid);


#endif


