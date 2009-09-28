
/*
 * scheduler.h
 */

#ifndef _scheduler_h
#define _scheduler_h

void block(int pid);

void unblock(int pid);

int pid next_process(void);

int pid create_process(void);

void desalojate(int pid);


#endif


