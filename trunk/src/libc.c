#include "../include/kc.h"
#include "../include/kernel.h"


/***************************************************************
*k_clear_screen
*
* Borra la pantalla en modo texto color.
****************************************************************/

void k_clear_screen() 
{
	char *vidmem = (char *) 0xb8000;
	unsigned int i=0;
	while(i < (80*25*2))
	{
		vidmem[i]='_';
		i++;
		vidmem[i]=WHITE_TXT;
		i++;
	};
}

/***************************************************************
*setup_IDT_entry
* Inicializa un descriptor de la IDT
*
*Recibe: Puntero a elemento de la IDT
*	 Selector a cargar en el descriptor de interrupcion
*	 Puntero a rutina de atencion de interrupcion	
*	 Derechos de acceso del segmento
*	 Cero
****************************************************************/

void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero) {
  item->selector = selector;
  item->offset_l = offset & 0xFFFF;
  item->offset_h = offset >> 16;
  item->access = access;
  item->cero = cero;
}

byte video_buffer[4000] = {0};
void put_char_at( byte c, int pos) {
	video_buffer[pos] = c;
	write(0, video_buffer, 4000);
}

size_t write(int fd, const void* buffer, size_t count) {
	int i;
	for ( i=0; i<count; i++) {
		_int_80_caller(WRITE, 0, *(((char *)buffer)+i));
	}
	return 0;
}
