#include "../include/video.h"
#include "../include/defs.h"	
#include "../include/stdio.h"	

byte bufferVideo[4000]={0};
byte backupAttr = BLUE_TXT;
int backupCursor = 1;

void
screenShow(byte c,int offset){
    bufferVideo[offset] = c;
    wpantalla(c,offset);
}

byte
fetch(void) {
    byte c;
    static int vcursor = 0;
    c = bufferVideo[vcursor++];
    if (vcursor >= TCIRC_SIZE*2)
	vcursor = 0;
    return c;
}

void
showMouseCursor(int pos){
    backupCursor = pos;
    backupAttr = bufferVideo[backupCursor];
    screenShow(MOUSE_CURSOR,backupCursor );
}

void
hideMouseCursor(int pos) {
   screenShow(backupAttr,backupCursor);
}
