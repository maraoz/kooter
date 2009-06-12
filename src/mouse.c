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

/*
 * Función que decodifica los datos enviados por el mouse.
 * Recibe el dato como un int b.
 */
void 
leomouse (int b){
	extern int interrupted;
	extern int tTicks;
	static point start,end;
	static int mouseClickIzq = 0, mouseClickDer = 0;

	interrupted = 1;
	tTicks=0;
	
	/*
	 * qty_int indica la cantidad de veces que interrumpió el mouse.
	 * Como se reciben 3 bytes por cada movimiento, entonces dependiendo
	 * del valor de qty_int se asigna a la variable que corresponda.
	 */
	switch(qty_int){
		case 0:	first = (byte)b;break;
		case 1:	second = (byte)b;break;
		case 2:	third = (byte)b;qty_int=-1;break;
		default: first = (byte)b;
	}
	qty_int++;
	
	/* Solo se updatea el estado del mouse 1 vez cada 3 interrupciones */
	if(!(qty_int)) {
	    hideMouseCursor();
	    mouseClickIzq = mouse.izq;
	    mouseClickDer = mouse.der;
	    updateMouse();

	    if (mouse.izq && !mouseClickIzq){
	    /* Si hizo click y no se estaba sosteniendo, 
	    entonces inicio la selección */
		start.x = mouse.pos.x;
		start.y = mouse.pos.y;
		hideSelection();
		showSelection(start, start);
	    }
	    if(mouse.izq && mouseClickIzq){
	    /* Si se hizo click y se estaba sosteniendo, 
	    entonces continuo la selección */
		end.x = mouse.pos.x;
		end.y = mouse.pos.y;
		hideSelection();
		showSelection(start,end);
	    }
	    if (!mouse.izq && mouseClickIzq) {
	    /* Se no se hizo click y se estaba sosteniendo,
	    entonces quiere decir que se solto y que se
	    debe copiar */
		hideSelection();
		end.x = mouse.pos.x;
		end.y = mouse.pos.y;
		showSelection(start,end);
		copy(start,end);
		hideSelection();
	    }
	    if(mouse.der && !mouseClickDer){
	    /* Si se hizo click derecho y no se estaba
	    sosteniendo entonces pego */
		hideSelection();
	    	paste();
		hideMouseCursor();
	    }
	    showMouseCursor(ptov(mouse.pos));
	}
	return;
}
/*
 * Función que updatea la estructura del mouse.
 */
void 
updateMouse(void){

    int dx,dy;
    point possible;

    /* Si se recibe un 1 en el 8 bit o en el bit 7 del primer 
    byte recibido del mouse, eso indica que hay overflow */
    if(first & 0x80 || first & 0x40){
 	puts("Ahhhh Overflow de mouse");
 	mouse_reset();
 	qty_int = 0;
    }
    else {
        /* Se setean los botones del mouse dependiendo si 
	están apretados o no */
	if(first & 0x02) {
	    mouse.der = 1;
	}
	else if(!(first & 0x02)) {
	    mouse.der = 0;
	}
	if(first & 0x01) {
	    mouse.izq = 1;
	}
	else if(!(first & 0x01)) {
	    mouse.izq = 0;
	}
	
	/* Se obtienen los signos de los desplazamientos del mouse en x e y */
	possible.x = ((first & 0x10)? -1: 1)*second;
	possible.y = ((first & 0x20)? 1: -1)*third;
	
	/* Se calcula el desplazamaiento del mouse sobre el eje x */
	if (possible.x < 0 ) {
	    dx = -1;
	} else if (possible.x > 0 ){
	    dx = 1;
	} else {
	    dx = 0;
	}
	
	/* Se calcula el desplazamaiento del mouse sobre el eje y */
	if (possible.y < 0 ) {
	    dy = -1;
	} else if (possible.y > 0 ){
	    dy = 1;
	} else {
	    dy = 0;
	}
    
	/* Se suman los desplazamientos a la posición anterior del mouse */
	mouse.pos.x += dx;
	mouse.pos.y += dy;
	
	
	/* Se controla que la posición del mouse no exceda la pantalla */
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

/*
 * Función que devuelve el atributo de la posición donde se encuentra
 * el mouse en pantalla.
 */
int
ptov(point pto) {
	return ((pto.x)+(pto.y)*80)*2+1;
}


/*
 * Función que copia desde el punto start hasta el punto
 * end. Copia tanto atributos como caracteres.
 */
void
copy(point start, point end){
    static byte tmpbuf[4000], auxi;
    int i,j,k=0;
    start.x;
    end.x;
    
    /* limpio el clibpoard*/
    for( i = 0 ; i < 4002 ; i++) {
	clipboard[i] = 0;
    }
    
    /* leo toda la pantalla y la dejo en tmpbuf*/
    read(PANTALLA_FD, tmpbuf, 4000);
    for(j = 0 ; j<abs((end.y  - start.y))+1 ; j++) {
    	for( i = 0 ; i<(abs(end.x  - start.x)+1)*2 ; i++) {
	    auxi = tmpbuf[((min(start.x,end.x)+min(start.y,end.y)*80))*2+i+j*160];
	    clipboard[k] = auxi;
	    k++;
	}
	/* agrego enters al final para copiar la selección tal cual aparecía en pantalla */
	clipboard[k] = '\n';
	k++;
	/* seteo el byte del atributo del \n */
	clipboard[k] = DEFAULT_TXT;
	k++;
    }
    /* agrego al final del clipboard un byte que indica que no hay nada más en él */
    clipboard[k] = 0xFF;
    k++;
    /* agrego al final del clipboard otro byte que indica que no hay nada más en él,
     * por si se lee sólo el byte del atributo o sólo el byte de carácter impreso.
     */
    clipboard[k] = 0xFF;
    k++;
}

/*
 * Función que pega lo que se hallaba en el 
 * clipboard en el buffer de teclado.
 */
void
paste(void){
    int k;
    /* Sólo toma los caracteres y no los atributos para pegar en el buffer de teclado */
    for(k = 0 ; clipboard[k*2] != 0xFF ; k++) {
 	writeToKeyboard(clipboard[k*2]);
    }
}

/* 
 * Función que devuelve el estado del mouse
 */
mouseSt 
mouse_now(void) {
    updateMouse();
    return mouse;
}
