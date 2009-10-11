
#include "../include/defs.h"
#include "../include/stdio.h"
#include "../include/util.h"
#include "../include/queue.h"



TTY tty[8];
int currentTTY = 0;
int focusedTTY = 0;

queue_t tty_kb_queues[8];

/****************************************************************
* init_ttys
* 
* inicializa las terminales y sus estructuras de control
*
****************************************************************/


void init_ttys() {
    currentTTY=0;
    int i;
    for(i=0;i<8;i++)
        tty[i].kb_buffer = &tty_kb_queues[i];
}


/***************************************************************
* str_cpy
*
* copia un string en otro tomando solo size caracteres
* 
****************************************************************/
void str_ncpy(byte * dest, byte * src, size_t size) {
    int i;
    for (i=0; i<size && src[i]; i++)  {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}


/***************************************************************
* page_roll
*
* scrollea la pantalla hacia arriba para dar lugar a m�s caracteres.
* deja el cursor en la posici�n correspondiente para seguir escribiendo
* sin notar el cambio.
* 
****************************************************************/
// byte screen_buffer[4160] = {};
byte blank_screen_buffer[4000] = {' ', DEFAULT_TXT, ' ', DEFAULT_TXT, ' ', DEFAULT_TXT};

void page_roll(int backwards) {


    tty[currentTTY].cursor = 0;

    check_offset('0',4000);
    write(PANTALLA_FD, tty[currentTTY].view+160,4000);
    tty[currentTTY].cursor -= 80;
    tty[currentTTY].cursor -= backwards;


    return;
}

/***************************************************************
* check_screen_scroll
*
* checkea si se necesita scrollear la pantalla antes de escribir
* offset cantidad de caracteres.
* 
****************************************************************/
void check_screen_scroll(int offset) {
    if (tty[currentTTY].cursor + offset >= 2000) {
        page_roll(2000-tty[currentTTY].cursor);
    }
    return;
}

/***************************************************************
* puts
*
* Escribe el string str en pantalla.
* 
****************************************************************/
void puts( const char * str ) {
    while (*str) {
        put_char(*str++);
    }
}

/***************************************************************
* putln
*
* Escribe el string str en pantalla. Agrega un enter al final.
* 
****************************************************************/
void putln( const char * str ) {
    puts(str);
    put_char('\n');
}

/***************************************************************
* gets
*
* Lee un string del teclado hasta el \n
* 
****************************************************************/
void gets( char str[] ) {
    byte c;
    int i=0;
    while ( (c = get_char()) != '\n') {
        if (c != '\x08')
            str[i++] = c;
        else
            --i<0? i=0 : i;
    }
    str[i] = '\0';
    return;
}



/***************************************************************
* put_char
*
* Escribe el caracter c en pantalla
* 
****************************************************************/
#define V_BUFFER_LENGTH 160

byte video_buffer[V_BUFFER_LENGTH] = {0};
byte clean_buffer[V_BUFFER_LENGTH] = {0};
static int vb_counter = 0;

void put_char( byte c) {
    check_screen_scroll(0);

    /* ENTER */
    if (c == '\n') {
        check_offset('1',vb_counter);
        write(PANTALLA_FD, video_buffer, vb_counter);
        check_screen_scroll(0);

        check_offset('2',V_BUFFER_LENGTH-(tty[currentTTY].cursor%80)*2);
        write(PANTALLA_FD, clean_buffer, V_BUFFER_LENGTH-(tty[currentTTY].cursor%80)*2 );
        vb_counter = 0;
        check_screen_scroll(0);
        return;
    }


    /* BACKSPACE */
    if (c == '\x08') {

        tty[currentTTY].cursor -= 1;
        check_offset('6',2);
        write(PANTALLA_FD, clean_buffer, 2);
        tty[currentTTY].cursor -= 1;

        return;
    }

    /* OTHER CHARACTERS */
    if (! (vb_counter < V_BUFFER_LENGTH)) {

        if (tty[currentTTY].cursor + vb_counter >= 2000) {
            int cursorBkp;
            check_screen_scroll(vb_counter);
            check_offset('7',vb_counter);
            write(PANTALLA_FD, video_buffer, vb_counter);
            vb_counter = 0;
        }
        else {
            check_screen_scroll(0);
            check_offset('3',vb_counter);
            write(PANTALLA_FD, video_buffer, vb_counter);
            vb_counter = 0;
        }
    }

    video_buffer[vb_counter] = c;
    video_buffer[vb_counter+1] = DEFAULT_TXT ;
    vb_counter += 2 ;
    

    
}


/***************************************************************
* get_char
*
* Lee un caracter en pantalla y lo devuelve
* 
****************************************************************/
#define K_BUFFER_LENGTH 1

byte keyboard_buffer[K_BUFFER_LENGTH] = {0};
int kb_counter = K_BUFFER_LENGTH;
int n_read = K_BUFFER_LENGTH;

byte get_char() {
    if ( kb_counter == n_read ) {
        // si no quedan caracteres sin leer del buffer
        n_read = 0;
        while (n_read == 0) {
            n_read = read(TECLADO_FD, keyboard_buffer, K_BUFFER_LENGTH);
        }

        kb_counter = 0;
    }
    return keyboard_buffer[kb_counter++];
    

    
}
/***************************************************************
* flush
*
* si queda algo en el buffer de put_char, lo imprime
* 
****************************************************************/
void flush() {
    
    if (tty[currentTTY].cursor + vb_counter >= 2000) {

        check_screen_scroll(vb_counter);
        check_offset('9',vb_counter);

        write(PANTALLA_FD, video_buffer, vb_counter);
        vb_counter = 0;
    } else {
        check_offset('4',vb_counter);
        write(PANTALLA_FD, video_buffer, vb_counter);
        vb_counter = 0;
    }
}

void borra_buffer() {
    vb_counter = 0;
    return;
}


/***************************************************************
* write
*
* Escribe count caracteres copiandolos de buffer al dispositivo
* descripto por el file descriptor fd. Llama a la int80h
*en B 
* Recibe como parametros:
* - File Descriptor
* - Buffer del source
* - Cantidad
****************************************************************/

size_t write(int fd, const void* buffer, size_t count) {
	int i;
	int offset;
	byte data;

	for ( i = 0 ; i<count; i++) {
		offset = i + tty[currentTTY].cursor*2;
		data = *((byte *)buffer+i);

		if (currentTTY == focusedTTY)
            _int_80_caller(WRITE, fd, offset, data);
		tty[currentTTY].view[offset] = data;

	}
	tty[currentTTY].cursor+=count/2;
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
    byte * b = (byte *)buffer;
    for ( i = 0 ; i<count; i++) {
        b[i] = _int_80_caller(READ, fd, i, 0);
        if (b[i] == 0xFF) { 
            break;
        }
    }
    return i;
}


/***************************************************************
*k_clear_screen
*
* Borra la pantalla en modo texto color.
****************************************************************/
char * BLANK_LINE = "                                                                                ";
void k_clear_screen() 
{
	tty[currentTTY].cursor = 0;
        check_offset('5',4000);
	write(PANTALLA_FD, blank_screen_buffer, 4000);
  	tty[currentTTY].cursor = 0;
}


/***************************************************************
* showSplashScreen
*
* Muestra la imagen de inicio
****************************************************************/
char * splash_screen[24] = {
"                                                                                ",
"                                                                                ",
"                                -===ccc,.                                       ",
"                                    =cc$$$$$$$$$$$$$$$cc.                       ",
"                                          ,,\"\"\"\"\"??$$$$$$$c                     ",
"        Kooter Kernel                  .,ccc$L,, ?bccc. \"\"?$$c     .             ",
"           v 1.0                    ,d\"\"\"$P\"\" $,z$$$$$P\",,,d$bcd$$$b.           ",
"                            ,z,,.  ??  ,$$\"  ,$$$$$$$ d$$$$$$$$$$$$$F           ",
"                            ?$$$$$$$cc,\"$$b=???3$$$$$ $$$$P\" 4P  \"?$\"           ",
"                             `\"?$$$$$$$????$$$$$$$$$$,\"\",,cdF'\"c                ",
"                               ,c,`\"\"\"\",$bc`?$$$$$$$$LF\"\"??$bcd$                ",
"                              d$$P\",c$$$$$$$$c\"$$$$$$$$$$$- $$$$                ",
"                             d$$b \"$$$$$$$$$$$c`$$$$$$$P\",c$$$$F                ",
"                            ,P\"z=`   ,,J$$$$$$$b $$$$$,zc$$$$$$F                ",
"                       :::, ?\",,cd$cc.`?$$$$$$$$ 4$$$??$$$$$$$F    .            ",
"                       `::',cbzccccc 4c$$$$$PF\"\"  .   \"\"\" ..::::::::            ",
"           ,cd$$$$$$$$$$ccd$$,,,`\"?$r ?\"\"..::::::::::::::::::::`::::            ",
"          4$$$$$$$$$$$$$$$$$$$$$$$ ?  ::::::::::::::::''``'` zc `::'            ",
"           \"$$$$$$$$$$$$$??$P\" $$P :!!```:::::::::`,,cc\",zc$$$$ :::             ",
"            \"?$$$$$$$$$P\"\"..:. ?\"   ....::::::::  d$$$$$$$$$$$$ :::             ",
"               \"??\"\"\" .::: - ..   `:::::::::::::: $$$$$$$$$$$$$ :::             ",
"                      :::::::::.::: ``:::::::::: .$$$$$$$$$$$$F ::              ",
"                      ::::::::::::::..:::::::::: <$$$$$$$$$$$P :::              ",
"                      :::::::::::::::::::::::::: $$$$$$$$$$$$\" ::               ",


};

void showSplashScreen() {
    int i;
    for (i = 0; i < 24; i++) {
        puts(splash_screen[i]);
    }
    

}
