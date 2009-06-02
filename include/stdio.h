#include "defs.h"


#ifndef _stdio_
    #define _stdio_

    #define WHITE_TXT 0x07 		// Atributo de video. Letras blancas, fondo negro
    #define BLUE_TXT 0x09       // Atributo de video. Letras azules, fondo negro
    #define INVISIBLE_TXT 0x00 	// Atributo de video. Letras negras, fondo negro

    /* el cursor de pantalla */
    
    int cursor;


    void puts( const char * str );

    void put_char( byte c);

    byte get_char();

    size_t write(int fd, const void* buffer, size_t count);

    /* Limpia la pantalla */
    void k_clear_screen();

    /* Muestra la imagen de inicio */
    void showSplashScreen();

#endif
