


#define WHITE_TXT 0x07 		// Atributo de video. Letras blancas, fondo negro
#define INVISIBLE_TXT 0x00 	// Atributo de video. Letras negras, fondo negro



void put_char_at( byte c, int pos);

size_t write(int fd, const void* buffer, size_t count);

/* Limpia la pantalla */
void k_clear_screen();

