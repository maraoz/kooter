#include "../include/video.h"
#include "../include/defs.h"	
#include "../include/stdio.h"	

byte bufferVideo[4000]={0};
byte backupSelection[2000]={BLUE_TXT};
byte backupAttr = BLUE_TXT;
point backupSelectionStart = {0,0};
point backupSelectionEnd = {0,0};
int backupCursor = 1;

void
screenShow(byte c,int offset){
	if(offset>=4000) {
		
		puts("ERROR: Me tiraste un offset muy grande\n");
	}
	else {
	bufferVideo[offset] = c;
	wpantalla(c,offset);

	}
}

byte
fetch(void) {
    byte c;
    static int vcursor = 0;
    c = bufferVideo[vcursor++];
    if (vcursor >= 4000)
	vcursor = 0;
    return c;
}

void
showMouseCursor(int pos){
    backupCursor = pos;
    backupAttr = bufferVideo[backupCursor]?bufferVideo[backupCursor]:BLUE_TXT;
    screenShow(MOUSE_CURSOR,backupCursor );
}

void
hideMouseCursor(void) {
   screenShow(backupAttr,backupCursor);
}

void
showSelection(point start, point end) {
    int i,j;
    backupSelectionStart = start;
    backupSelectionEnd = end;
    int auxi;
    for( i = 0 ; i<abs(start.y-end.y)+1 ; i++) {
	for( j = 0; j<abs(start.x - end.x)+1 ; j++) {
		auxi = ((min(start.x,end.x))+(min(start.y,end.y))*80+i*80+j);
		backupSelection[auxi] = bufferVideo[auxi*2+1]?bufferVideo[auxi*2+1]:BLUE_TXT;
		screenShow(MOUSE_CURSOR,auxi*2+1);
	}
    }	 
}

void
hideSelection(void){
    int i,j;
    for( i = 0 ; i<abs(backupSelectionStart.y-backupSelectionEnd.y)+1 ; i++) {
	for( j = 0; j<abs(backupSelectionStart.x - backupSelectionEnd.x)+1 ; j++) {
	    screenShow(backupSelection[(min(backupSelectionStart.x,backupSelectionEnd.x))
	    +(min(backupSelectionStart.y,backupSelectionEnd.y))*80+i*80+j],
	    (((min(backupSelectionStart.x,backupSelectionEnd.x))+
	    (min(backupSelectionStart.y,backupSelectionEnd.y))*80+i*80+j)*2+1));
	}
    }	 

}
