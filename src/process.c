#include "../include/allocator.h"
#include "../include/queue.h"
#include "../include/defs.h"
#include "../include/kc.h"
#include "../include/command.h"
#include "../include/kasm.h"
#include "../include/process.h"


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
    for (i=1; i<MAX_PROCESSES; i++) {
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
create_process(int (*funcion)(), int pages_qty, int argc, char **argv, int gid, int priority, int background,int tty)
{
    int i;
    context_t new_proc;

//     put_char('0'+(char)funcion);
//     flush();
//     _Cli();
    new_proc.process.pid = get_new_pid();
    new_proc.process.gid = gid;
    new_proc.process.background = background;
     new_proc.tty = tty;


    /* inicializar stack */
    new_proc.page = new_proc.ESP = (dword)palloc(pages_qty);
    new_proc.ESP += pages_qty*4096-1;
    new_proc.ESP = create_new_stack(funcion,argc,argv,new_proc.ESP,end_process);
    new_proc.SS = 0x10;
   
    
    bcp[new_proc.process.pid] = new_proc;


    desalojate(new_proc.process.pid);
//     _Sti();
    return new_proc.process;
}

dword
create_new_stack(int(*funcion)(),int argc,char** argv,dword bottom, void(*end_proc)())
{
    STACK_FRAME* frame= (STACK_FRAME*)(bottom-sizeof(STACK_FRAME));
    frame->EBP=0;
    frame->ESP=(dword)&(frame->EIP);
    frame->EIP=(dword)funcion;
    frame->CS=0x08;

    frame->EFLAGS=0x200;
    frame->retaddr=end_proc;
    frame->argc=argc;
    frame->argv=argv;
    return (dword)frame;
}

void
process_creator(){
    
    create_process(pepe,1,1,(char**)0,1,1,FALSE,0);

    create_process(pepe2,1,1,(char**)0,1,1,FALSE,0);
// 
//     create_process(pepe2,1,1,(char**)0,1,1,FALSE,0);
//     
//     create_process(pepe2,1,1,(char**)0,1,1,FALSE,0);

}

int pepe() {
    char * m = (char *) 0x0B8000;
    while (1) {
        int i;
        for (i=0; i<4000; i++)
            m[i] = '4';
    }
}

int pepe2() {
    char * m = (char *) 0x0B8000;
    while (1) {
        int i;
        for (i=0; i<4000; i++)
            m[i] = '6';
    }
}

int pepe3() {
    int a=0;
    while(1){
        a++;
        if(a%50000){
            put_char('c'); 
            a=a%50000;
        }
    }
}

int bnm() {
    int a=0;
    while(1){
        a++;
        if(a%50000){
            put_char('d'); 
            a=a%50000;
        }
    }
}

