#include "../include/video.h"
#include "../include/defs.h"	
#include "../include/stdio.h"	

byte bufferVideo[4000]={0};
byte backupSelection[2000]={DEFAULT_TXT};
byte backupAttr = DEFAULT_TXT;
point backupSelectionStart = {0,0};
point backupSelectionEnd = {0,0};
int backupCursor = 1;

/*
 * Función que se encarga de agregar el caracter 
 * c en el buffer de video en la posición offset y que 
 * llama a la función wpantalla para que se muestre en 
 * el caracter en pantalla en la posicón offset de la misma.
 */
void
screenShow(byte c,int offset){
	if(!(offset>=4000 && offset<0)) {
	    bufferVideo[offset] = c;
	    wpantalla(c,offset);
	}
}

/*
 * Función que se encarga de obtener datos
 * del buffer de video. Tiene un índice
 * interno con el cual se mueve por todo el buffer.
 */
byte
fetch(void) {
    byte c;
    static int vcursor = 0;
    c = bufferVideo[vcursor++];
    if (vcursor >= 4000)
	vcursor = 0;
    return c;
}

/*
 * Función que se encarga de mostrar el
 * cursor del mouse en la posición pos.
 * Hace un backup del atributo de pos y de pos
 * para luego poder recuperarlos.
 */
void
showMouseCursor(int pos){
    backupCursor = pos;
    /* Si el atributo es 0 entonces le asigno DEFAULT_TXT*/
    backupAttr = bufferVideo[backupCursor]?bufferVideo[backupCursor]:DEFAULT_TXT;
    screenShow(MOUSE_CURSOR,backupCursor );
}


/*
 * Función que esconde el cursor del mouse
 * de la posición que se encuentra backupeada
 * y le restaura el atributo que se encontraba allí.
 */
void
hideMouseCursor(void) {
   screenShow(backupAttr,backupCursor);
}


/*
 * Función que se encarga de mostrar la selección
 * desde la posición start hasta la posición end
 * cursor del mouse en la posición pos.
 */
void
showSelection(point start, point end) {
    int i,j;
    backupSelectionStart = start; /* backup de la posición inicial */
    backupSelectionEnd = end;	/* backup de la posición final */
    int auxi;
    for( i = 0 ; i<abs(start.y-end.y)+1 ; i++) {
	for( j = 0; j<abs(start.x - end.x)+1 ; j++) {
		auxi = ((min(start.x,end.x))+(min(start.y,end.y))*80+i*80+j);
		/* Si el atributo es 0 entonces le asigno DEFAULT_TXT*/
		backupSelection[auxi] = bufferVideo[auxi*2+1]?bufferVideo[auxi*2+1]:DEFAULT_TXT;
		screenShow(MOUSE_CURSOR,auxi*2+1);
	}
    }	 
}


/*
 * Función que esconde el la selección que estaba determinada
 * por backupSelectionStart y backupSelectionEnd
 * y le restauran los atributos que se encontraban allí.
 */
void
hideSelection(void){
    int i,j;
    int auxi;
    for( i = 0 ; i<abs(backupSelectionStart.y-backupSelectionEnd.y)+1 ; i++) {
	for( j = 0; j<abs(backupSelectionStart.x - backupSelectionEnd.x)+1 ; j++) {
	    auxi = (min(backupSelectionStart.x,backupSelectionEnd.x)) 
	    +(min(backupSelectionStart.y,backupSelectionEnd.y))*80+i*80+j;
	    screenShow(backupSelection[auxi],
	    ((auxi)*2+1));
	}
    }	 

}
