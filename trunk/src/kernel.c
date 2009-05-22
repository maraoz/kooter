#include "../include/kasm.h"
#include "../include/defs.h"

DESCR_INT idt[0x80];			/* IDT de 128 entradas*/
IDTR idtr;				/* IDTR */

int tickpos=640;

void int_08() {

	_int_80_caller(1, 0, 0x30);
}



/**********************************************
kmain() 
Punto de entrada de cóo C.
*************************************************/

kmain() 
{

        int i,num;

/* Borra la pantalla. */ 

	k_clear_screen();


/* CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ0    */

        setup_IDT_entry (&idt[0x08], 0x08, (dword)&_int_08_hand, ACS_INT, 0);
	

/* CARGA DE IDT CON LA RUTINA DE ATENCION DE LA INT80h   */

        setup_IDT_entry (&idt[0x80], 0x10, (dword)&_int_80_hand, ACS_INT, 0);



/* Carga de IDTR    */

	idtr.base = 0;  
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;
	
	_lidt (&idtr);	

	_Cli();
/* Habilito interrupcion de timer tick*/

        _mascaraPIC1(0xFE);
        _mascaraPIC2(0xFF);
        
	_Sti();

        while(1)
        {
        }
	
}

