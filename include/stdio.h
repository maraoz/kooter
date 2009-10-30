#include "defs.h"


#ifndef _stdio_
    #define _stdio_



    #define GRAY_TXT 0x07 	// Atributo de video. Letras blancas, fondo negro
    #define LIGHT_BLUE_TXT 0x09       // Atributo de video. Letras azules, fondo negro
    #define INVISIBLE_TXT 0x00 	// Atributo de video. Letras negras, fondo negro
    #define RED_TXT 0x04
    #define RED_ALL 0x44
    #define GREEN_TXT 0x02
    #define ORANGE_TXT 0x06
    #define DARK_BLUE_TXT 0x01
    #define WHITE_TXT 0x0F
    #define YELLOW_TXT 0x0E

    #define DEFAULT_TXT LIGHT_BLUE_TXT // Atributo para el video por default.
    #define MOUSE_CURSOR 0x30	//  Atributo para el cursor del mouse.


    /* cambia la vista a la tty indicada */
    int switch_tty(int new_tty);

    /* mueve el cursor a la posición indicada en la terminal actual */ 
    void move_cursor_to(int pos);
    
    /* mueve el cursor a la posicion indicada dentro de la linea actual */
    void move_cursor_inline(int offset);

    /* muestra la pantalla entera de la terminal */
    void update_screen();

    /* escribe un caracter en stdout*/
    void put_char( byte c);

    /* borrar el buffer de put_char */
    void borra_buffer();

    /* lee un caracter del teclado */
    byte get_char();

    /* imprime en pantalla lo que no se imprimi� del buffer de put_char */
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
    

#endif
