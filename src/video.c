#include "../include/video.h"
#include "../include/defs.h"	
byte bufferVideo[TCIRC_SIZE*2]={0};


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

    