#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/kc.h"
#include "../include/stdio.h"
#include "../include/util.h"
#include "../include/keyboard.h"



DESCR_INT idt[0x81];			/* IDT de 129 entradas*/
IDTR idtr;				/* IDTR */

byte c='a';
void int_08() {

	return;

}

byte leoteclado (){
      byte a;
      a = _int_80_caller(READ, 2, 0, 0);
      a = ktoa(a);
      
      if(a >= 0x20) {
	put_char(a);
	flush();
      }
      return a;
}

byte leomouse (){
      byte a;
      a = _int_80_caller(READ, 1, 0, 0);
      a = a&0x01;
      if(a==1)
	put_char('1');
      else
	put_char('0');
      //a = mtoa(a);
      
      return a;
}



/**********************************************
kmain() 
Punto de entrada de cóo C.
*************************************************/

kmain() 
{



/* CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ0    */
	setup_IDT_entry (&idt[0x08], 0x08, (dword)&_int_08_hand, ACS_INT, 0);
	

/* CARGA DE IDT CON LA RUTINA DE ATENCION DE LA INT80h   */
	setup_IDT_entry (&idt[0x80], 0x08, (dword)&_int_80_hand, ACS_INT, 0);
	
/* CARGA DE IDT CON LA RUTINA DE ATENCION DE LA IRQ9   */
	setup_IDT_entry (&idt[0x09], 0x08, (dword)&_int_09_hand, ACS_INT, 0);
	
/* CARGA DE IDT CON LA RUTINA DE ATENCION DE LA IRQ12   */
	setup_IDT_entry (&idt[0x0C], 0x08, (dword)&_int_12_hand, ACS_INT, 0);



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
    
/* Borra la pantalla. */ 
    
//      k_clear_screen();
     
/* Para probar la splashscreen comentar arriba y descomentar esta. */ 
//         showSplashScreen();



/*     Ejemplo de uso de flush()     */
    int i;
    for (i=0; i<23; i++)
        put_char('c');
    flush();
    
    while(1) {
        
    }

	
}
