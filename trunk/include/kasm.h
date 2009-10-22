/*********************************************
kasm.h

************************************************/

#include "defs.h"

unsigned int    _read_msw();

void            _lidt (IDTR *idtr);

void		_mascaraPIC1 (byte mascara);  /* Escribe mascara de PIC1 */
void		_mascaraPIC2 (byte mascara);  /* Escribe mascara de PIC2 */

void		_Cli(void);        /* Deshabilita interrupciones  */
void		_Sti(void);	 /* Habilita interrupciones  */

void		_int_08_hand();      /* Timer tick */

void		_int_80_hand();      /* Subrutina de atencion de Interrupcion 80h*/

void		_int_09_hand();      /* Subrutina de atencion de Interrupcion 09h, teclado*/

void		_int_74_hand();      /* Subrutina de atencion de Interrupcion 12h, mouse*/

byte		_int_80_caller(byte write_or_read, byte file_descriptor, dword offset, byte dato);

void		_debug (void);

void		enable_mouse(void);

void		mouse_reset(void);

void		wpantalla(word c, int offset);

void		disable_text_cursor(void);

void            init_pagination(void);

void        Disable();

void        Enable();

void        _mifunc();

void		_int_00_hand();      /* Subrutina de atencion de Interrupcion 80h*/
void		_int_01_hand();      /* Subrutina de atencion de Interrupcion 80h*/
void		_int_02_hand();      /* Subrutina de atencion de Interrupcion 80h*/
void		_int_03_hand();      /* Subrutina de atencion de Interrupcion 80h*/
void		_int_04_hand();      /* Subrutina de atencion de Interrupcion 80h*/
void		_int_05_hand();      /* Subrutina de atencion de Interrupcion 80h*/
void		_int_06_hand();      /* Subrutina de atencion de Interrupcion 80h*/
void		_int_07_hand();      /* Subrutina de atencion de Interrupcion 80h*/
void		_int_0A_hand();      /* Subrutina de atencion de Interrupcion 80h*/
void		_int_0B_hand();      /* Subrutina de atencion de Interrupcion 80h*/
void		_int_0C_hand();      /* Subrutina de atencion de Interrupcion 80h*/
void		_int_0D_hand();      /* Subrutina de atencion de Interrupcion 80h*/
void		_int_0E_hand();      /* Subrutina de atencion de Interrupcion 80h*/
void		_int_0F_hand();      /* Subrutina de atencion de Interrupcion 80h*/
void		_int_10_hand();      /* Subrutina de atencion de Interrupcion 80h*/
void		_int_11_hand();      /* Subrutina de atencion de Interrupcion 80h*/
