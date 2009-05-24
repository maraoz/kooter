

#include "../include/defs.h"
#include "../include/stdio.h"


int cursor = 0;

/***************************************************************
* puts
*
* Escribe el string str en pantalla. Agrega un enter al final.
* 
****************************************************************/
void puts( char * str ) {
    while (*str) {
        put_char(*str++);
    }
    put_char('\n');
}



/***************************************************************
* put_char
*
* Escribe el caracter c en pantalla
* 
****************************************************************/
#define BUFFER_LENGTH 160

byte video_buffer[BUFFER_LENGTH] = {0};
int vb_counter = 0;

void put_char( byte c) {
	if (vb_counter < BUFFER_LENGTH && c!='\n') {
		video_buffer[vb_counter] = c;
		video_buffer[vb_counter+1] = WHITE_TXT ;
		vb_counter += 2 ;
	}
	else {
		vb_counter = 0;
		write(WRITE, video_buffer, BUFFER_LENGTH);
	}
	
}

/***************************************************************
* write
*
* Escribe count caracteres copiandolos de buffer al dispositivo
* descripto por el file descriptor fd. Llama a la int80h
*
* Recibe como parametros:
* - File Descriptor
* - Buffer del source
* - Cantidad
****************************************************************/
size_t write(int fd, const void* buffer, size_t count) {
	int i;
	int limit = count + cursor;
	for ( i = cursor ; i<limit; i++) {
		_int_80_caller(WRITE, fd, i, *(((byte *)buffer)+(i-cursor)));
	}
	cursor+=count;
	return 0;
}


/***************************************************************
* read
*
* lee count caracteres copiandolos del dispositivo descripto por
* el file descriptor fd al buffer. Llama a la int80h 
*
* Recibe como parametros:
* - File Descriptor
* - Buffer a donde escribir
* - Cantidad
****************************************************************/
size_t read(int fd, void* buffer, size_t count) {
    int i;
    for ( i = cursor ; i<limit; i++) {
        buffer[i] = _int_80_caller(READ, fd, i, *(((byte
                        *)buffer)+(i-cursor)));
    }
}

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
		vidmem[i]=WHITE_TXT +2 ;
		i++;
	};
}

