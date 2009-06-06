#include "../include/video.h"
#include "../include/defs.h"	
#include "../include/stdio.h"	

byte bufferVideo[4000]={0};
byte backupAttr = BLUE_TXT;
int backupCursor = 1;

void
screenShow(byte c,int offset){
    if(offset>4000) {
	puts("Me tiraste un offset muy grande");
	while(1);
    }
    bufferVideo[offset] = c;
    wpantalla(c,offset);
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
    backupAttr = bufferVideo[backupCursor];
    screenShow(MOUSE_CURSOR,backupCursor );
}

void
hideMouseCursor(void) {
   screenShow(backupAttr,backupCursor);
}