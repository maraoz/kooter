#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/kc.h"
#include "../include/stdio.h"
#include "../include/util.h"
#include "../include/keyboard.h"
#include "../include/shell.h"
#include "../include/video.h"
#include "../include/command.h"
#include "../include/allocator.h"     
#include "../include/process.h"     
#include "../include/scheduler.h"     
#include "../include/files.h"

DESCR_INT idt[0x81];            /* IDT de 129 entradas*/
IDTR idtr;                      /* IDTR */

int interrupted = 1;

/* proceso actual que esta corriendo */
pid_t current_process = 0;

/* la terminal actual que muestra Kooter */
extern int focusedTTY;

extern TTY tty[8]; // vector con las 8 terminales

context_t bcp[MAX_PROCESSES]; /* BCP para todos los procesos que van a estar para switchear */
int ticks = 0;


void
SaveESP(dword ESP){
    bcp[current_process].ESP = ESP;
}

dword
GetTemporaryESP(){
    return bcp[0].ESP;
}

dword
LoadESP(){
    dword ESP = bcp[current_process].ESP;
    return ESP;
}


dword int_08(dword ESP)
{
//     down_p(bcp[current_process].page, bcp[current_process].page_qty);
    scheduler();
//     up_p(bcp[current_process].page, bcp[current_process].page_qty);
    return  bcp[current_process].ESP;
}



/**********************************************
kmain()
Punto de entrada de código C.
*************************************************/
kmain() 
{

    init_pagination();

/* Habilito el mouse */
    enable_mouse();


    
/* Deshabilito el cursor de texto */
    disable_text_cursor();

/*Excepciones*/
    setup_IDT_entry (&idt[0x00], 0x08, (dword)&_int_00_hand, ACS_CODE, 0);
    setup_IDT_entry (&idt[0x01], 0x08, (dword)&_int_01_hand, ACS_CODE, 0);
    setup_IDT_entry (&idt[0x02], 0x08, (dword)&_int_02_hand, ACS_CODE, 0);
    setup_IDT_entry (&idt[0x03], 0x08, (dword)&_int_03_hand, ACS_CODE, 0);
    setup_IDT_entry (&idt[0x04], 0x08, (dword)&_int_04_hand, ACS_CODE, 0);
    setup_IDT_entry (&idt[0x05], 0x08, (dword)&_int_05_hand, ACS_CODE, 0);
    setup_IDT_entry (&idt[0x06], 0x08, (dword)&_int_06_hand, ACS_CODE, 0);
    setup_IDT_entry (&idt[0x07], 0x08, (dword)&_int_07_hand, ACS_CODE, 0);
    setup_IDT_entry (&idt[0x0A], 0x08, (dword)&_int_0A_hand, ACS_CODE, 0);
    setup_IDT_entry (&idt[0x0B], 0x08, (dword)&_int_0B_hand, ACS_CODE, 0);
    setup_IDT_entry (&idt[0x0C], 0x08, (dword)&_int_0C_hand, ACS_CODE, 0);
    setup_IDT_entry (&idt[0x0D], 0x08, (dword)&_int_0D_hand, ACS_CODE, 0);
    setup_IDT_entry (&idt[0x0E], 0x08, (dword)&_int_0E_hand, ACS_CODE, 0);
    setup_IDT_entry (&idt[0x0F], 0x08, (dword)&_int_0F_hand, ACS_CODE, 0);
    setup_IDT_entry (&idt[0x10], 0x08, (dword)&_int_10_hand, ACS_CODE, 0);
    setup_IDT_entry (&idt[0x11], 0x08, (dword)&_int_11_hand, ACS_CODE, 0);
    
    
    
    
    
/* CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ0    */
    setup_IDT_entry (&idt[0x08], 0x08, (dword)&_int_08_hand, ACS_INT, 0);
    
/* CARGA DE IDT CON LA RUTINA DE ATENCION DE LA INT80h   */
    setup_IDT_entry (&idt[0x80], 0x08, (dword)&_int_80_hand, ACS_INT, 0);
    
/* CARGA DE IDT CON LA RUTINA DE ATENCION DE LA IRQ1   */
    setup_IDT_entry (&idt[0x09], 0x08, (dword)&_int_09_hand, ACS_INT, 0);

/* CARGA DE IDT CON LA RUTINA DE ATENCION DE LA IRQ12   */
    setup_IDT_entry (&idt[0x74], 0x08, (dword)&_int_74_hand, ACS_INT, 0);


/* Carga de IDTR    */
    idtr.base = 0;  
    idtr.base +=(dword) &idt;
    idtr.limit = sizeof(idt)-1;
    
    _lidt (&idtr);    

    _Cli();

// seteo las interrupciones en 1
    interrupted=1;
    tTicks = 0;
/* Habilito interrupcion de timer tick y del teclado y del mouse en el PIC*/
    _mascaraPIC1(0xF8);
    _mascaraPIC2(0xEF);

    init_pids();
    init_scheduler();

//     k_clear_screen();


    init_ttys();
    fs_init();
    bcp[0].process.isAlive = TRUE;
    str_ncpy(bcp[0].process.name,"init", 20);
    process_creator();
    
    _Sti();
    while(1){
        block_me();
    }
}

