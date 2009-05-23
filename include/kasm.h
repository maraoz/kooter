/*********************************************
kasm.h

************************************************/

#include "defs.h"
#include "kernel.h"


unsigned int    _read_msw();

void            _lidt (IDTR *idtr);

void		_mascaraPIC1 (byte mascara);  /* Escribe mascara de PIC1 */
void		_mascaraPIC2 (byte mascara);  /* Escribe mascara de PIC2 */

void		_Cli(void);        /* Deshabilita interrupciones  */
void		_Sti(void);	 /* Habilita interrupciones  */

void		_int_08_hand();      /* Timer tick */

void		_int_80_hand();      /* Subrutina de atencion de Interrupcion 80h*/

void		_int_09_hand();      /* Subrutina de atencion de Interrupcion 09h, teclado*/

int 		_int_80_caller(byte write_or_read, byte file_descriptor, dword offset, byte dato);


void		_debug (void);

