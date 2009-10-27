#include "../include/allocator.h"
#include "../include/queue.h"
#include "../include/defs.h"
#include "../include/kc.h"
#include "../include/command.h"
#include "../include/kasm.h"
#include "../include/process.h"
#include "../include/shell.h"
#include "../include/files.h"
#include "../include/stdio.h"


queue_t available_pids, used_pids;
queue_t * available_pids_q, * used_pids_q;

#define asdkhskladhs007 while(1);

extern pid_t current_process; /* proceso actual que esta corriendo */
extern context_t bcp[MAX_PROCESSES];

extern TTY tty[8];
/**
 * init_pids
 * inicializa las estructuras generadoras de pids
 */
int init_pids(void) {
    available_pids_q = &available_pids;
    used_pids_q = &used_pids;

    queue_init(available_pids_q);
    int i;
    for (i=1; i<MAX_PROCESSES; i++) {
        enqueue(available_pids_q, i);
    }

    queue_init(used_pids_q);
    enqueue(used_pids_q, 0);

    return 0;
}

/**
 * get_new_pid
 * devuelve el siguiente pid sin usar
 */

pid_t get_new_pid(void) {
    pid_t new_pid = dequeue(available_pids_q);
    enqueue(used_pids_q, new_pid);
    return new_pid;
}

/**
 * end_process
 * Funcion que destruye los procesos
 */
void
end_process()
{
    char * m = (char *)0xB8000;
    dequeue_element(used_pids_q, current_process);
    enqueue(available_pids_q, current_process);
    pfree(bcp[current_process].page, bcp[current_process].page_qty);
    if(bcp[current_process].process.background == FALSE){
        unblock(bcp[current_process].dad_pid);
    }
    bcp[current_process].process.isAlive = FALSE;
    context_switch();
    asdkhskladhs007
}

/**
 * get_current_tty()
 */
int get_current_tty() {
    return bcp[current_process].tty;
}

/**
 * Funcion que crea un nuevo proceso
 */
process_t
create_process(int (*funcion)(), int pages_qty, int argc, char **argv, int gid, int priority, int background,int tty, pid_t dad_pid, char * name)
{
    int i,a;
    char * m = (char *) 0xB8000;
    context_t new_proc;

//     _Cli();
    new_proc.process.pid = get_new_pid();
    new_proc.process.gid = gid;
    new_proc.process.background = background;
    new_proc.process.isAlive = TRUE;
    str_ncpy(new_proc.process.name,name, 20);
    
    new_proc.tty = tty;
    new_proc.dad_pid = dad_pid;
    
    /* inicializar stack */
    new_proc.page_qty = pages_qty;
    new_proc.page = palloc(pages_qty);
//     new_proc.page = palloc();
    new_proc.ESP = (dword)new_proc.page;
    new_proc.ESP += (pages_qty*PAGE_SIZE)-1;
    new_proc.ESP = create_new_stack(funcion,argc,argv,new_proc.ESP,end_process);
    new_proc.SS = 0x10;
   

    bcp[new_proc.process.pid] = new_proc;




//     pfree(new_proc.page, pages_qty);

//     palloc(1);


    a = desalojate(new_proc.process.pid);

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
    create_process((int(*)(void))shell,4,1,(char**)0,1,1,FALSE,0,current_process,"shell 1");
    create_process((int(*)(void))shell,1,1,(char**)0,1,1,FALSE,1,current_process,"shell 2");
    create_process((int(*)(void))shell,1,1,(char**)0,1,1,FALSE,2,current_process,"shell 3");
    create_process((int(*)(void))shell,1,1,(char**)0,1,1,FALSE,3,current_process,"shell 4");
    create_process((int(*)(void))shell,1,1,(char**)0,1,1,FALSE,4,current_process,"shell 5");
    create_process((int(*)(void))shell,1,1,(char**)0,1,1,FALSE,5,current_process,"shell 6");
    create_process((int(*)(void))shell,1,1,(char**)0,1,1,FALSE,6,current_process,"shell 7");
    create_process((int(*)(void))shell,1,1,(char**)0,1,1,FALSE,7,current_process,"shell 8");

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

