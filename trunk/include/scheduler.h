
/*
 * scheduler.h
 */

#ifndef _scheduler_h
#define _scheduler_h

int init_scheduler(void);

int block(int pid);

int unblock(int pid);

int next_process(void);

void desalojate(int pid);


#endif


