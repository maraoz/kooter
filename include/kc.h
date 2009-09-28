/**********************
 kc.h
**********************/
#include "defs.h"

#ifndef _kc_
#define _kc_




typedef struct process_context {
    
    
} pcontext_t;

/* Tiempo de espera */
void wait(int time);


/* Inicializa la entrada del IDT */
void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero);

#endif
