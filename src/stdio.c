
#include "../include/defs.h"
#include "../include/stdio.h"



extern int cursor = 0;





/***************************************************************
* page_roll
*
* 
* 
****************************************************************/
void page_roll() {
    
}

/***************************************************************
* puts
*
* Escribe el string str en pantalla. Agrega un enter al final.
* 
****************************************************************/
void puts( const char * str ) {
    while (*str) {
        put_char(*str++);
    }
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
        str[i++] = c;
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
    /* ENTER */
    if (c == '\n') {
        write(PANTALLA_FD, video_buffer, vb_counter);
        write(PANTALLA_FD, clean_buffer, V_BUFFER_LENGTH-vb_counter);
        vb_counter = 0;
        return;
    }
    
    /* BACKSPACE */
    if (c == '\x08') {
        cursor -= 1;
        write(PANTALLA_FD, clean_buffer, 1);
        cursor -= 1;
        return;
        
    }
    
    /* OTHER CHARACTERS */
    if (! (vb_counter < V_BUFFER_LENGTH)) {
        write(PANTALLA_FD, video_buffer, vb_counter);
        vb_counter = 0;
    }

    video_buffer[vb_counter] = c;
    video_buffer[vb_counter+1] = BLUE_TXT ;
    vb_counter += 2 ;

    
}


/***************************************************************
* get_char
*
* Lee un caracter en pantalla y lo devuelve
* 
****************************************************************/
#define K_BUFFER_LENGTH 10

byte keyboard_buffer[K_BUFFER_LENGTH] = {0};
int kb_counter = K_BUFFER_LENGTH;
int n_read = K_BUFFER_LENGTH;

byte get_char() {
    if ( kb_counter == n_read ) {
        do {
	    n_read = read(TECLADO_FD, keyboard_buffer, K_BUFFER_LENGTH);
	}
        while (n_read == 0);
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
    write(PANTALLA_FD, video_buffer, vb_counter);
    vb_counter = 0;
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
	for ( i = 0 ; i<count; i++) {
		_int_80_caller(WRITE, fd, i + cursor, *((byte *)buffer+i));
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
    byte * b = (byte *)buffer;
    for ( i = 0 ; i<count; i++) {
        b[i] = _int_80_caller(READ, fd, i, 0);
        if (b[i] == 0) { 
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
	int i;
	for (i = 0; i < 25*80; i++) {
        put_char(' ');
	}
}


/***************************************************************
* showSplashScreen
*
* Muestra la imagen de inicio
****************************************************************/
char * splash_screen[25] = {
"                                                                                ",
"                                -===ccc,.                                       ",
"                                    =cc$$$$$$$$$$$$$$$cc.                       ",
"                                          ,,\"\"\"\"\"??$$$$$$$c                     ",
"        Kooter Kernel                  .,ccc$L,, ?bccc. \"\"?$$c     .             ",
"           v 0.1                    ,d\"\"\"$P\"\" $,z$$$$$P\",,,d$bcd$$$b.           ",
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
"                                                                                ",
" >>>                                                                            "
};

void showSplashScreen() {
    int i;
    for (i = 0; i < 25; i++) {
        puts(splash_screen[i]);
    }

}
