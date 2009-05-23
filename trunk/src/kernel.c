#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/kc.h"
#include "../include/kernel.h"
#include "../include/stdio.h"


DESCR_INT idt[0x81];			/* IDT de 129 entradas*/
IDTR idtr;				/* IDTR */


byte c='a';
void int_08() {

	
	put_char(c++);
	
	
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
        setup_IDT_entry (&idt[0x80], 0x08, (dword)&_int_80_hand, ACS_INT, 0);



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

