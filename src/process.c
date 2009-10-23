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
extern context_t bcp[MAX_PROCESSES];

extern TTY tty[8];
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
 * end_process
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
 * get_current_tty()
 */
int get_current_tty() {
    return bcp[current_process.pid].tty;
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
//     new_proc.page = palloc(pages_qty);
    new_proc.page = palloc();
    new_proc.ESP = (dword)new_proc.page;
    new_proc.ESP += (pages_qty*PAGE_SIZE)-1;
    new_proc.ESP = create_new_stack(funcion,argc,argv,new_proc.ESP,end_process);
    new_proc.SS = 0x10;
   
    
    bcp[new_proc.process.pid] = new_proc;




//     pfree(new_proc.page, pages_qty);

//     palloc(1);




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
    
    //create_process(int (*funcion)(), int pages_qty, int argc, char **argv, int gid, int priority, int background,int tty)
    create_process(pepe,4,1,(char**)0,1,1,FALSE,0);

    create_process(pepe2,4,1,(char**)0,1,1,FALSE,1);

    create_process(pepe3,4,1,(char**)0,1,1,FALSE,2);

    create_process(pepe4,4,1,(char**)0,1,1,FALSE,3);
//     
//     create_process(pepe2,1,1,(char**)0,1,1,FALSE,0);

}

int pepe() {
    
    while (1) {
        put_char('1');
        flush();
    }
}

int pepe2() {
    
    while (1) {
    put_char('2');
    flush();
    }
}

int pepe3() {

    
    while (1) {put_char('3');
    flush();
    }

}
int pepe4() {

    while (1) {
    put_char('4');
    flush();
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

