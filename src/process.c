#include "../include/allocator.h"
#include "../include/queue.h"
#include "../include/defs.h"
#include "../include/kc.h"
#include "../include/command.h"
#include "../include/kasm.h"
#include "../include/process.h"
#include "../include/shell.h"
#include "../include/files.h"
#include "../include/shell_proc.h"
#include "../include/scheduler.h"
#include "../include/stdio.h"



queue_t available_pids, used_pids;
queue_t * available_pids_q, * used_pids_q;
queue_t * ready_processes_q;

#define asdkhskladhs007xD while(1);

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
    _Cli();
    enqueue(used_pids_q, new_pid);
    _Sti();
    return new_pid;
}

/**
 * end_process
 * Funcion que destruye los procesos
 */
void
end_process()
{   
    _Cli();
// //         char * m = (char*)0xB8000;
        
    if(dequeue_element(used_pids_q, current_process)==-1){
//         m[current_process*2]='0';
//         m[current_process*2+1]='0';
    }
    enqueue(available_pids_q, current_process);
    pfree(bcp[current_process].page, bcp[current_process].page_qty);
    if(bcp[current_process].process.background == FALSE){
        wake_up_father((bcp[current_process].dad_pid));
    }
    bcp[current_process].process.isAlive = FALSE;
    _Sti();
    context_switch();
    asdkhskladhs007xD
}

/**
 * kill
 * Funcion que mata procesos
 */
void
kill(pid_t pid)
{   
    
    int i,apid;
    if( pid > MAX_PROCESSES || pid <= 0){
        if(pid == -1){
            putln("No se puede matar a ese proceso.");
        }
        return;
    }
    _Cli();
    for(i=0;i<MAX_PROCESSES;i++){
        if(bcp[i].process.isAlive == TRUE && (bcp[i].process.pid == pid || bcp[i].process.gid == pid || bcp[i].dad_pid == pid)){
            apid = bcp[i].process.pid;
            dequeue_element(ready_processes_q, apid);
            dequeue_element(used_pids_q, apid);
            enqueue(available_pids_q, apid);
            pfree(bcp[apid].page, bcp[apid].page_qty);
            if(bcp[apid].process.background == FALSE){
                wake_up_father((bcp[apid].dad_pid));
            } 
            bcp[apid].process.isAlive = FALSE;
        }
    }
    _Sti();

}

/**
 * get_current_tty()
 */
int get_current_tty() {
    return bcp[current_process].tty;
}

char * getProcessName(pid) {
    return bcp[pid].process.name;
}
#define getMyName() getProcessName(current_process)

char * getProcessState( pid_t pid) {
    if (bcp[pid].process.isBlocked) {
        return "BLOCKED";
    } else {
        if (pid == current_process) {
            return "RUNNING";
        } else {
            return "WAITING";
        }
    }
}

#define getMyState() getProcessState(current_process)

/**
 * Funcion que crea un nuevo proceso
 */
void
create_process(int (*funcion)(), int pages_qty, int argc, char **argv, int gid, int priority, int background,int tty, pid_t dad_pid, char * name)
{
    _Cli();
    int i,a;
    context_t new_proc;    



    new_proc.process.pid = get_new_pid();

    if(gid == -1)
        new_proc.process.gid = new_proc.process.pid;
    new_proc.process.background = background;
    new_proc.process.isAlive = TRUE;
    str_ncpy(new_proc.process.name, name, 20);

    new_proc.tty = tty;
    new_proc.dad_pid = dad_pid;
    
    /* inicializar stack */
    new_proc.page_qty = pages_qty;
    new_proc.page = palloc(pages_qty);
    new_proc.ESP = (dword)new_proc.page;
    new_proc.ESP += (pages_qty*PAGE_SIZE)-1;
    new_proc.ESP = create_new_stack(funcion,argc,argv,new_proc.ESP,end_process);
    new_proc.SS = 0x10;
   

    bcp[new_proc.process.pid] = new_proc;


    a = desalojate(new_proc.process.pid);

    _Sti();

    return;
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
    create_process((int(*)(void))shell,4,1,(char**)0,-1,0,FALSE,0,current_process,"shell 1");
    create_process((int(*)(void))shell,4,1,(char**)0,-1,0,FALSE,1,current_process,"shell 2");
    create_process((int(*)(void))shell,4,1,(char**)0,-1,0,FALSE,2,current_process,"shell 3");
    create_process((int(*)(void))shell,4,1,(char**)0,-1,0,FALSE,3,current_process,"shell 4");
    create_process((int(*)(void))shell,4,1,(char**)0,-1,0,FALSE,4,current_process,"shell 5");
    create_process((int(*)(void))shell,4,1,(char**)0,-1,0,FALSE,5,current_process,"shell 6");
    create_process((int(*)(void))shell,4,1,(char**)0,-1,0,FALSE,6,current_process,"shell 7");
    create_process((int(*)(void))shell,4,1,(char**)0,-1,0,FALSE,7,current_process,"shell 8");

}

// int pepe() {
//     while (1) {
//     }
// }
// 
// int pepe2() {
//     while (1) {
//     put_char('2');
//     flush();
//     }
// }
// 
// int pepe3() {
// 
//     while (1) {put_char('3');
//     flush();
//     }
// 
// }
// int pepe4() {
// 
//     while (1) {
//     put_char('4');
//     flush();
//     }
// 
// }
// 
// int bnm() {
//     int a=0;
//     while(1){
//         a++;
//         if(a%50000){
//             put_char('d'); 
//             a=a%50000;
//         }
//     }
// }

