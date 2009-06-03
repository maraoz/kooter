#include "../include/kc.h"
#include "../include/defs.h"
#include "../include/kasm.h"
#include "../include/stdio.h"
#include "../include/mouse.h"
#include "../include/util.h"

mouseSt mouse = {0,0,{0,0}};
byte first, second, third;
byte clipboard[TCIRC_SIZE];
byte bkpCursorPos = DEFAULT_TXT;

void 
leomouse (int b){
	static int qty_int=0;
	static point start,end;
	static int mouseClickIzq = 0;

	
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
	    updateMouse();
	    showMouseCursor();
	    if (mouse.izq && !mouseClickIzq){
		start.x = mouse.pos.x;
		start.y = mouse.pos.y;
	    }
	    if (!mouse.izq && mouseClickIzq) {
		end.x = mouse.pos.x;
		end.y = mouse.pos.y;
		copy(start,end);
	    }
	    if(mouse.der) {
		hideMouseCursor();
		paste();
		showMouseCursor();
	    }
		
	}
	return;
}

void 
updateMouse(void){

    int dx, dy, possibleDX, possibleDY;

    if(first & 0x80 || first & 0x40){
	puts("Overflow de mouse");
    }
    else {
	if(first & 0x02) {
	    mouse.der = 1;
	}
	else if(!(first & 0x02)) {
	    mouse.der = 0;
	}
	if(first & 0x04) {
	    puts("Hizo click del medio");
	}
	if(first & 0x01) {
	    mouse.izq = 1;
	}
	else if(!(first & 0x01)) {
	    mouse.izq = 0;
	}
	
    possibleDX = ((first & 0x10)? -1: 1)*second;
    possibleDY = ((first & 0x20)? 1: -1)*third;
    if (possibleDX < 0 ) {
	dx = -1;
    } else if (possibleDX > 0 ){
	dx = 1;
    } else {
	dx = 0;
    }
    
    if (possibleDY < 0 ) {
	dy = -1;
    } else if (possibleDY > 0 ){
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
    return (((pto.x)*2 + (pto.y)*160)%2?((pto.x)*2 + (pto.y)*160) : ((pto.x)*2 + (pto.y)*160))+1;
}

void
hideMouseCursor(void) {
    _int_80_caller(WRITE, PANTALLA_FD, ptov(mouse.pos), bkpCursorPos );
}

void
showMouseCursor(void) {
    bkpCursorPos = _int_80_caller(READ, PANTALLA_FD, ptov(mouse.pos), 0);
    _int_80_caller(WRITE, PANTALLA_FD, ptov(mouse.pos), MOUSE_CURSOR);
}


void
copy(point start, point end){
    static byte tmpbuf[4000], auxi;
    int i,j,k=0;
    for( i = 0 ; i < TCIRC_SIZE ; i++) {
	clipboard[i] = 0;
    }
    read(PANTALLA_FD, tmpbuf, 4000);
    for(j = 0 ; j<abs(end.y +1 - start.y) ; j++) {
    	for( i = 0 ; i<abs(end.x +1 - start.x) ; i++) {
	    auxi = tmpbuf[(min(start.x,end.x)*2+160*min(start.y,end.y))+i*2+j*160];
	    if(auxi != 0) {
		clipboard[k] = auxi;
		k++;
	    }
	}
    }
}

void
paste(void){
    puts(clipboard);
    flush();
}


mouseSt 
mouse_now(void) {
    updateMouse();
    return mouse;
}