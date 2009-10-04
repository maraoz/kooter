/*
 * Funcion que destruye los procesos
 */
void
end_process()
{
    setStatus(current_process, FREE);     /* designo a la tarea como libre */
}

/*
 * Funcion que crea un nuevo proceso
 */

process_t
create_process(int (*funcion)(int,char**), int pages_qty, int argc, void **argv, int gid, int priority, int background)
{
    int i;
    context_t new_proc;


    _Cli();
    new_proc.process.pid = get_next_pid();
    new_proc.process.gid = gid;
    new_proc.process.background = background;

    set_status(new_proc.process, SUSPENDED);

    /* inicializar stack */

    
    new_proc.page = new_proc.ESP = palloc(pages_qty);
    new_proc.ESP += pages_qty*(1024*pages_qty)-1;
    new_proc.ESP = create_new_stack(funcion,argc,argv,new_proc.ESP,end_proc);
    new_proc.SS = 0x10;

    set_status(new_proc.process, READY);
    _Sti();
    return task;
}

dword
create_new_stack(dword(*proceso)(int,char**),int argc,char** argv,dword bottom, void(*end_proc)())
{
    STACK_FRAME* frame= (STACK_FRAME*)(bottom-sizeof(STACK_FRAME);
    frame->EBP=0;
    frame->EIP=(dword)proceso;
    frame->CS=0x08;

    frame->EFLAGS=0;
    frame->retaddr=end_proc;
    frame->argc=argc;
    frame->argv=argv;
    return (dword)frame;
}