#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/kc.h"
#include "../include/stdio.h"
#include "../include/util.h"
#include "../include/keyboard.h"
#include "../include/shell.h"
#include "../include/video.h"	

DESCR_INT idt[0x81];			/* IDT de 129 entradas*/
IDTR idtr;				/* IDTR */

int interrupted = 1;

process_t current_process; /* proceso actual que esta corriendo */
context_t bcp[MAX_CANT_PROCESS]; /* BCP para todos los procesos que van a estar para switchear */

void int_08(dword EDI,dword ESI, dword EBP, dword SS_interrupt, dword EBX, dword EDX, dword ECX, dword EAX, dword ES, dword DS, dword SS, dword ret_value, dword CS, dword IP, dword CFLAGS)
{
    
    bcp[current_process.pid].pid = current_process.pid;
    bcp[current_process.pid].process = current_process;
    bcp[current_process.pid].EDI = EDI;
    bcp[current_process.pid].ESI = ESI;
    bcp[current_process.pid].EBP = EBP;
    bcp[current_process.pid].SS = SS;
    bcp[current_process.pid].EBX = EBX;
    bcp[current_process.pid].EDX = EDX;
    bcp[current_process.pid].ECX = ECX;
    bcp[current_process.pid].EAX = EAX;
    bcp[current_process.pid].ES = ES;
    bcp[current_process.pid].DS = DS;
    bcp[current_process.pid].CS = CS;
    bcp[current_process.pid].IP = IP;
    bcp[current_process.pid].CFLAGS = CFLAGS;

    desalojate(current_process.pid);
    
    current_process.pid = next_process();

    current_process = bcp[current_process].process;

    EDI = bcp[current_process.pid].EDI;
    ESI = bcp[current_process.pid].ESI;
    EBP = bcp[current_process.pid].EBP;
    SS = bcp[current_process.pid].SS;
    EBX = bcp[current_process.pid].EBX;
    EDX = bcp[current_process.pid].EDX;
    ECX = bcp[current_process.pid].ECX;
    EAX = bcp[current_process.pid].EAX;
    ES = bcp[current_process.pid].ES;
    DS = bcp[current_process.pid].DS;
    CS = bcp[current_process.pid].CS;
    IP = bcp[current_process.pid].IP;
    CFLAGS = bcp[current_process.pid].CFLAGS;
    
	return;
}


/**********************************************
kmain()
Punto de entrada de código C.
*************************************************/

kmain() 
{

/* Habilito el mouse */
	enable_mouse();

/* Deshabilito el cursor de texto */
	disable_text_cursor();
	
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
/* Habilito interrupcion de timer tick y del teclado y del mouse*/
	_mascaraPIC1(0xF8);
    /*          1111 1000   */
	_mascaraPIC2(0xEF);
    /*          1110 1111   */
	
	_Sti();


// muestra la splashScreen

        showSplashScreen();

/* espero a que la pueda ver */

        wait(10);

/* Borra la pantalla. */ 
        k_clear_screen();
        


	shell();

}

