#include "../include/kc.h"
#include "../include/defs.h"
#include "../include/kasm.h"
#include "../include/stdio.h"
#include "../include/keyboard.h"
#include "../include/scheduler.h"

extern pid_t current_process;
extern TTY tty[8];
extern int focusedTTY;
extern context_t bcp[MAX_PROCESSES];

extern int interrupted;
extern int tTicks;

tcirc teclator={0,0,0,{0}};

/*
 * FunciÔøΩn que recibe un scancode k y lo 
 * decodifica en ascii.
 */
void leoteclado (int k){
    byte c;

    tTicks=0;

    if (!(teclator.qty_used >= TCIRC_SIZE)){
    /* Si le buffer de teclado no est√° lleno entonces procedo
    a decodificar el scancode en ascii */
        c = ktoa(k);

        if(c != 0xFF) {
        /* Si es un caracter soportado por Kooter,
        lo agrego al buffer de teclado */
            if(interrupted==0)
            /* Si estaba en el salva pantallas, vuelvo 
            y no dejo el ascii en el buffer de teclado */
                interrupted = 1;
            else if(c != 0x00){
                if(teclator.next_write == TCIRC_SIZE)
                /* Implementaci√≥n del teclado circular */
                teclator.next_write = 0;

                teclator.tcircular[teclator.next_write] = c;
                teclator.next_write++;
                teclator.qty_used++;

                // agregado para primitivas bloqueantes (manu)
                enqueue(tty[focusedTTY].kb_buffer, c);
                if(isFs(c)){
                    flush();
                    switch_tty(3-focusedTTY); /*le paso como parametro la terminal a la que quiero switchear */
//                     while(1);
                }
                int pid;
                for (pid=0;pid<MAX_PROCESSES;pid++) {
                    if(is_blocked(pid) && bcp[pid].tty==focusedTTY)
                        unblock(pid);
                }
            }
        }
    }
}

byte ktoa(int c){
	static int shift = 0;
	/* matriz que contiene en la primer fila los ascii soportados por Kooter sin "shiftear"
	 * y en la segunda fila los ascii soportados por Kooter "shifteados".
	 * El scancode que se recibe al presionar una tecla sirve de √≠ndice para cada fila. */
	int const keystroke[2][83]={
        {0x00/*Esc*/,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39
        ,0x30,0x2D,0x3D,0x08,0x00/*Tab*/,0x71,0x77,0x65,0x72
        ,0x74,0x79,0x75,0x69,0x6F,0x70,0x5B,0x5D,0x0A
        ,0x00/*ctrl*/,0x61,0x73,0x64,0x66,0x67,0x68,0x6A
        ,0x6B,0x6C,0x3B,0x27,0x60,0x00/*Lshifht*/,0x5C
        ,0x7A,0x78,0x63,0x76,0x62,0x6E,0x6D,0x2C,0x2E
        ,0x2F,0x00/*RShift*/,0x2A,0x00/*alt*/,0x20
        ,0x00/*caps*/,0xf1,0xf2,0xf3,0xf4
        ,0xf5,0xf6,0xf7,0xf8,0x03,0x02
        ,0x00/*num*/,0x00/*scrl*/,0x00,0x01,0x00
        ,0x2D,0x04,0x00,0x02,0x2B,0x4F00,0x03
        ,0x00,0x00,0x00},

        {0x00/*Esc*/,0x21,0x40,0x23,0x24,0x25,0x5E,0x26,0x2A,0x28
        ,0x29,0x5F,0x2B,0x08,0x00/*Tab*/,0x51,0x57,0x45,0x52
        ,0x54,0x59,0x55,0x49,0x4F,0x50,0x7B,0x7D,0x0D
        ,0x00/*ctrl*/,0x41,0x53,0x44,0x46,0x47,0x48,0x4A
        ,0x4B,0x4C,0x3A,0x22,0x7E,0x00/*Lshifht*/,0x7C
        ,0x5A,0x58,0x43,0x56,0x42,0x4E,0x4D,0x3C,0x3E
        ,0x3F,0x00/*RShift*/,0x00/*INT 5 PRT SCREEN*/,0x00/*alt*/,0x20
        ,0x00/*caps*/,0x00,0x00,0x50,0x00
        ,0x00,0x00,0x01,0x04,0x03,0x02
        ,0x00/*num*/,0x00/*scrl*/,0x00,0x00,0x00
        ,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        ,0x00,0x00,0x00}};

    /* Setea el shift a partir del scancode recibido */
    if( c == 0x2A || c == 0x36)
	shift = 1;
    if( c == 0x2A+0x80 || c == 0x36+0x80)
	shift = 0;

    /* Si el scancode corresponde a soltar una tecla retorno 0xFF */
    if(c>=0x81)
      return (byte)0xFF;
    return (byte)keystroke[shift][c-1];
}

int
isFs(int c){
    return 'a' == c;
//     return (c == 0xf1 || c == 0xf2 || c == 0xf3
//         || c == 0xf4 || c == 0xf5 || c == 0xf6
//         || c == 0xf7 || c ==0xf8 );
}

/* 
 * Funci√≥n que devuelve el siguiente caracter del buffer de teclado.
 * Si esta vac√≠o el buffer, devuelve 0xFF como c√≥digo de error.
 */
byte next_char (){
    byte a;
    if(teclator.qty_used > 0) {
        a=teclator.tcircular[teclator.next_read];
        teclator.next_read++;
        teclator.qty_used--;

        //TODO: esto es lo ˙nico q sirve, lo dem·s est· deprecated
        int currentTTY = get_current_tty();
        a = dequeue(tty[currentTTY].kb_buffer);
        //TODO: esto es lo ˙nico q sirve, lo dem·s est· deprecated


        if(teclator.next_read >= TCIRC_SIZE)
            teclator.next_read = 0;
    }
    else {
        a = 0xFF;
        block_me();
    }
    return a;
}

/*
 * Funci√≥n que escribe en el buffer de teclado el byte c.
 */
void 
writeToKeyboard(byte c)
{
    if(teclator.next_write == TCIRC_SIZE)
                teclator.next_write = 0;
    if(!(teclator.qty_used >= TCIRC_SIZE)){
	teclator.tcircular[teclator.next_write] = c;
	teclator.next_write++;
	teclator.qty_used++;
    }
}
