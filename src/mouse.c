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
	if(!(qty_int))
	    updateMouse();
	return;
}

void 
updateMouse(void){
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
	    mouse.pos.x += (first & 0x01)*second;
	    mouse.pos.y += (first & 0x02)*third;
	}
	return;
}     


mouseSt 
mouse_now(void) {
    updateMouse();
    return mouse;
}