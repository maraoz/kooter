#include "../include/kc.h"
#include "../include/defs.h"
#include "../include/kasm.h"
#include "../include/stdio.h"
#include "../include/mouse.h"

mouseSt mouse = {0,0,{0,0}};
byte first, second, third;

void 
leomouse (int b){
	static int qty_int=0;

	switch(qty_int){
		case 0:	first = (byte)b;break;
		case 1:	second = (byte)b;break;
		case 2:	third = (byte)b;qty_int=-1;break;
		default: first = (byte)b;
	}
	qty_int++;
	if(!(qty_int)) {
	    _int_80_caller(WRITE, PANTALLA_FD, ptov(mouse.pos), DEFAULT_TXT);
	    updateMouse();
	    _int_80_caller(WRITE, PANTALLA_FD, ptov(mouse.pos), 0x30);
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
        } else if (possibleDYx > 0 ){
            dy = 1;
        } else {
            dy = 0;
        }
        
	    mouse.pos.x += dx;
	    mouse.pos.y += dy;
	   
	    
	    if(mouse.pos.x > 80){
		mouse.pos.x = 80;
	    }
	    else if(mouse.pos.x < 0){
		mouse.pos.x = 0;
	    }
	    if(mouse.pos.y > 25){
		mouse.pos.y = 25;
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

mouseSt 
mouse_now(void) {
    updateMouse();
    return mouse;
}