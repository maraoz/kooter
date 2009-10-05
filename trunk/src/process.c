#include "../include/allocator.h"
#include "../include/queue.h"
#include "../include/defs.h"
#include "../include/kc.h"


queue_t available_pids;
queue_t * available_pids_q;
extern process_t current_process; /* proceso actual que esta corriendo */
extern pid_t focus;
extern context_t bcp[MAX_PROCESSES];

/**
 * init_pids
 * inicializa las estructuras generadoras de pids
 */
int init_pids(void) {
    available_pids_q = &available_pids;
    queue_init(available_pids_q);
    int i;
    for (i=0; i<MAX_PROCESSES; i++) {
        enqueue(available_pids_q, i);
    }
    return 0;
}

/**
 * get_new_pid
 * devuelve el siguiente pid sin usar
 */
pid_t get_new_pid(void) {
    return dequeue(available_pids_q);
}

/**
 * Funcion que destruye los procesos
 */
void
end_process()
{
    // TODO: faltan cosas para terminar de matar al proceso
    enqueue(available_pids_q, current_process.pid);

    // TODO: falta meter el nuevo proceso


}

/**
 * Funcion que crea un nuevo proceso
 */
process_t
create_process(int (*funcion)(int,char**), int pages_qty, int argc, char **argv, int gid, int priority, int background)
{
    int i;
    context_t new_proc;


//     _Cli();
    new_proc.process.pid = get_new_pid();
    new_proc.process.gid = gid;
    new_proc.process.background = background;

    desalojate(new_proc.process.pid);
    block(new_proc.process.pid);

    /* inicializar stack */
    new_proc.page = new_proc.ESP = (dword)palloc(pages_qty);
    new_proc.ESP += pages_qty*4096-1;
    new_proc.ESP = create_new_stack(funcion,argc,argv,new_proc.ESP,end_process);
    new_proc.SS = 0x10;

    bcp[new_proc.process.pid] = new_proc;
    unblock(new_proc.process.pid);
//     _Sti();
    return new_proc.process;
}

dword
create_new_stack(int(*proceso)(int,char**),int argc,char** argv,dword bottom, void(*end_proc)())
{
    STACK_FRAME* frame= (STACK_FRAME*)(bottom-sizeof(STACK_FRAME));
    frame->EBP=0;
    frame->EIP=(dword)proceso;
    frame->CS=0x08;

    frame->EFLAGS=0;
    frame->retaddr=end_proc;
    frame->argc=argc;
    frame->argv=argv;
    return (dword)frame;
}


