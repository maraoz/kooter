
/* 
 * scheduler.c
 */


void block(int pid);

void unblock(int pid);

int pid next_process(void);

int pid create_process(void);

void desalojate(int pid);

