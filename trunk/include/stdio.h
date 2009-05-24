
#ifndef _stdio_h_
#define _stdio_h_


#define WHITE_TXT 0x07 		// Atributo de video. Letras blancas, fondo negro
#define INVISIBLE_TXT 0x00 	// Atributo de video. Letras negras, fondo negro

void puts( char * str );

void put_char( byte c);

size_t write(int fd, const void* buffer, size_t count);

/* Limpia la pantalla */
void k_clear_screen();

#endif