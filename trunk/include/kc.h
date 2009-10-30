/**********************
 kc.h
**********************/
#include "defs.h"

#ifndef _kc_
#define _kc_

#include "../include/kasm.h"

#define MAX_PROCESSES 42


/* Tiempo de espera */
void wait(int time);

void        _Cli(void); /* deshabilita interrupciones anidablemente */
void        _Sti(void); /* habilita interrupciones anidablemente */

/* Inicializa la entrada del IDT */
void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,
        byte cero);


boolean runningInBackground();

#endif
