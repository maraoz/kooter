#include "../include/kc.h"
#include "../include/defs.h"
#include "../include/kasm.h"
#include "../include/stdio.h"
#include "../include/mouse.h"
#include "../include/util.h"
#include "../include/video.h"

mouseSt mouse = {0,0,{0,0}};
byte first, second, third;
byte clipboard[TCIRC_SIZE];
byte bkpCursorPos = DEFAULT_TXT;
int qty_int = 0;

void 
leomouse (int b){
	extern int interrupted;
	extern int tTicks;
	static point start,end;
	static int mouseClickIzq = 0, mouseClickDer = 0;

	interrupted = 1;
	tTicks=0;
	switch(qty_int){
		case 0:	first = (byte)b;break;
		case 1:	second = (byte)b;break;
		case 2:	third = (byte)b;qty_int=-1;break;
		default: first = (byte)b;
	}
	qty_int++;
	if(!(qty_int)) {
	    hideMouseCursor();
	    mouseClickIzq = mouse.izq;
	    mouseClickDer = mouse.der;
	    updateMouse();


	    if (mouse.izq && !mouseClickIzq){
		start.x = mouse.pos.x;
		start.y = mouse.pos.y;
		hideSelection();
		showSelection(start, start);
	    }
	    if(mouse.izq && mouseClickIzq){
		end.x = mouse.pos.x;
		end.y = mouse.pos.y;
		hideSelection();
		showSelection(start,end);
	    }
	    if (!mouse.izq && mouseClickIzq) {
		hideSelection();
		end.x = mouse.pos.x;
		end.y = mouse.pos.y;
		showSelection(start,end);
		copy(start,end);
		hideSelection();
	    }
	    if(mouse.der && !mouseClickDer){
		hideSelection();
	    	paste();
		hideMouseCursor();
	    }
	    showMouseCursor(ptov(mouse.pos));
	}
	return;
}

void 
updateMouse(void){

    int dx,dy;
    point possible;

    if(first & 0x80 || first & 0x40){
 	puts("Ahhhh Overflow de mouse");
 	mouse_reset();
 	qty_int = 0;
    }
    else {
	if(first & 0x02) {
	    mouse.der = 1;
	}
	else if(!(first & 0x02)) {
	    mouse.der = 0;
	}
// 	if(first & 0x04) {
// 	    puts("Hizo click del medio");
// 	}
	if(first & 0x01) {
	    mouse.izq = 1;
	}
	else if(!(first & 0x01)) {
	    mouse.izq = 0;
	}
	    
	possible.x = ((first & 0x10)? -1: 1)*second;
	possible.y = ((first & 0x20)? 1: -1)*third;
	if (possible.x < 0 ) {
	    dx = -1;
	} else if (possible.x > 0 ){
	    dx = 1;
	} else {
	    dx = 0;
	}
	
	if (possible.y < 0 ) {
	    dy = -1;
	} else if (possible.y > 0 ){
	    dy = 1;
	} else {
	    dy = 0;
	}
    
	mouse.pos.x += dx;
	mouse.pos.y += dy;
	
	
	if(mouse.pos.x > 79){
	    mouse.pos.x = 79;
	}
	else if(mouse.pos.x < 0){
	    mouse.pos.x = 0;
	}
	if(mouse.pos.y > 24){
	    mouse.pos.y = 24;
	}
	else if(mouse.pos.y < 0){
	    mouse.pos.y = 0;
	}

    }
    return;
}     

int
ptov(point pto) {
//     return (((pto.x)*2 + (pto.y)*160)%2?((pto.x)*2 + (pto.y)*160) : ((pto.x)*2 + (pto.y)*160))+1;
	return ((pto.x)+(pto.y)*80)*2+1;
}

/*
void
moveMouseCursor(point pto){
    hideMouseCursor(mouse.pos);
    if(pto.x > 0) {
	mouse.pos.x++;
    }
    else if(pto.x < 0){
	mouse.pos.x--;
    }
    if(pto.y > 0) {
	mouse.pos.y++;
    }
    else if(pto.y < 0){
	mouse.pos.y--;
    }
    showMouseCursor(mouse.pos);
}*/


void
copy(point start, point end){
    static byte tmpbuf[4000], auxi;
    int i,j,k=0;
    start.x;
    end.x;
    for( i = 0 ; i < 4000 ; i++) {
	clipboard[i] = 0;
    }
    read(PANTALLA_FD, tmpbuf, 4000);
    for(j = 0 ; j<abs((end.y  - start.y))+1 ; j++) {
    	for( i = 0 ; i<(abs(end.x  - start.x)+1)*2 ; i++) {
	    auxi = tmpbuf[((min(start.x,end.x)+min(start.y,end.y)*80))*2+i+j*160];
	    clipboard[k] = auxi;
	    k++;
	}
	clipboard[k] = '\n';
	k++;
	clipboard[k] = DEFAULT_TXT;
	k++;
    }
    clipboard[k] = 0xFF;
    k++;
    clipboard[k] = 0xFF;
    k++;
}

void
paste(void){
    int k;
    for(k = 0 ; clipboard[k*2] != 0xFF ; k++) {
// 	if(!(k%2)) {
 	writeToKeyboard(clipboard[k*2]);
// 	}
// 	screenShow(clipboard[k],k);
    }
}


mouseSt 
mouse_now(void) {
    updateMouse();
    return mouse;
}
