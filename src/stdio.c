

#include "../include/defs.h"
#include "../include/stdio.h"




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
static int vb_counter = 0;

void put_char( byte c) {
	if (! (vb_counter < BUFFER_LENGTH && c!='\n')) {
		write(PANTALLA_FD, video_buffer, vb_counter);
        vb_counter = 0;
	}
    
    video_buffer[vb_counter] = c;
    video_buffer[vb_counter+1] = BLUE_TXT ;
    vb_counter += 2 ;

	
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

int cursor = 0;

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
    }
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

/* Muestra la imagen de inicio */



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
