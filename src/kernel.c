#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/kc.h"
#include "../include/stdio.h"
#include "../include/util.h"
#include "../include/keyboard.h"


DESCR_INT idt[0x81];			/* IDT de 129 entradas*/
IDTR idtr;				/* IDTR */




void int_08() {

	return;

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
/* Habilito interrupcion de timer tick y del teclado y del mouse*/
	_mascaraPIC1(0xF8);
    /*          1111 1000   */
	_mascaraPIC2(0xEF);
    /*          1110 1111   */
	
	_Sti();
	int a = 0;
/* Borra la pantalla. */ 
    
//      k_clear_screen();
     
/* Para probar la splashscreen comentar arriba y descomentar esta. */ 
//         showSplashScreen();




    enable_mouse();
/*    puts("hola");
    char buf[100];
    gets(buf);
    puts("//mucha magia//");
    puts(buf);
    puts("chau");
    flush();*/
    
    
    
//     put_char('a');
    while(1) {

    }

    puts("allalalalalalaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa2");
    flush();

}

