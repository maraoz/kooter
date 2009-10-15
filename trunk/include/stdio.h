#include "defs.h"


#ifndef _stdio_
    #define _stdio_



    #define WHITE_TXT 0x07 	// Atributo de video. Letras blancas, fondo negro
    #define BLUE_TXT 0x09       // Atributo de video. Letras azules, fondo negro
    #define INVISIBLE_TXT 0x00 	// Atributo de video. Letras negras, fondo negro
    #define RED_TXT 0x04
    #define RED_ALL 0x44
    #define YELLOW_TXT 0x02

    #define DEFAULT_TXT BLUE_TXT // Atributo para el video por default.
    #define MOUSE_CURSOR 0x30	//  Atributo para el cursor del mouse.


    /* muestra la pantalla entera de la terminal */
    void update_screen();

    /* escribe un caracter en stdout*/
    void put_char( byte c);

    /* borrar el buffer de put_char */
    void borra_buffer();

    /* lee un caracter del teclado */
    byte get_char();

    /* imprime en pantalla lo que no se imprimió del buffer de put_char */
    void flush();

    /* imrpime en pantalla el string str */
    void puts( const char * str );

    /* Lee un string del teclado hasta que el usuario presione enter */
    void gets( char str[] );

    /* escribe el string y agrega una bajada de linea al final */
    void putln( const char * str );





    /* primitiva para escribir dispositivos */
    size_t write(int fd, const void* buffer, size_t count);

    /* primitiva para leer desde dispositivos */
    size_t read(int fd, void* buffer, size_t count);




    /* Copia n chars de un string a otro */
    void str_ncpy(byte * dest, byte * src, size_t size);

    /* Limpia la pantalla */
    void k_clear_screen();

    /* Muestra la imagen de inicio */
    void showSplashScreen();


    

#endif
