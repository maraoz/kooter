#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/kc.h"
#include "../include/stdio.h"


DESCR_INT idt[0x81];			/* IDT de 129 entradas*/
IDTR idtr;				/* IDTR */


byte c='a';
void int_08() {

	
	//puts("Hola!");	
}

byte leoteclado (){
      byte a;
      put_char(a);
      a=_int_80_caller(READ, 2, 0, 0);
      switch(a) {
      case 0x10: a='q';break;
      case 0x11: a='w';break;
      case 0x12: a='e';break;
      case 0x13: a='r';break;
      case 0x14: a='t';break;
      case 0x15: a='y';break;
      case 0x16: a='u';break;
      case 0x17: a='i';break;
      case 0x18: a='o';break;
      case 0x19: a='p';break;
      case 0x1E: a='a';break;
      case 0x1F: a='s';break;
      case 0x20: a='d';break;
      case 0x21: a='f';break;
      case 0x22: a='g';break;
      case 0x23: a='h';break;
      case 0x24: a='j';break;
      case 0x25: a='k';break;
      case 0x26: a='l';break;
      case 0x2C: a='z';break;
      case 0x2D: a='x';break;
      case 0x2E: a='c';break;
      case 0x2F: a='v';break;
      case 0x30: a='b';break;
      case 0x31: a='n';break;
      case 0x32: a='m';break;
      default: a='0';
      }
      if(a!='0')
	put_char(a);
      return a;
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
	
/* CARGA DE IDT CON LA RUTINA DE ATENCION DE LA IRQ9   */
	setup_IDT_entry (&idt[0x09], 0x08, (dword)&_int_09_hand, ACS_INT, 0);



/* Carga de IDTR    */
	idtr.base = 0;  
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;
	
	_lidt (&idtr);	

	_Cli();
/* Habilito interrupcion de timer tick y del teclado*/

	_mascaraPIC1(0xFC);
	_mascaraPIC2(0xFF);
	
	_Sti();

	while(1)
	{
	}
	
}

