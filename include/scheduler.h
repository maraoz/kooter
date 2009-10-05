
/*
 * scheduler.h
 */

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
 * next_process
 * devuelve el pid del siguiente proceso a ejecutar
 * (segun el algoritmo del scheduler)
 */
pid_t next_process(void);

/**
 * desalojate
 * vuelve a poner el proceso en la cola de readys
 */
int desalojate(int pid);


#endif


