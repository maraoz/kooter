

#include "../include/defs.h"
#include "../include/stdio.h"


int cursor = 0;


byte video_buffer[100] = {0};
int vb_counter = 0;

void put_char( byte c) {
	
	
	if (vb_counter < 100) {
		video_buffer[vb_counter] = c;
		video_buffer[vb_counter+1] = WHITE_TXT ;
		vb_counter += 2 ;
	}
	else {
		vb_counter = 0;
		write(WRITE, video_buffer, 100);
	}
	
}

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

