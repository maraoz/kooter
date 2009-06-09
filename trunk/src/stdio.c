
#include "../include/defs.h"
#include "../include/stdio.h"



int cursor = 0;


void memcpy(byte * a, byte * b, size_t size) {
    while (size--) {
        a[size] = b[size];
    }
}


/***************************************************************
* page_roll
*
* 
* 
****************************************************************/
byte screen_buffer[4160] = {};
byte blank_screen_buffer[4000] = {0};

void page_roll() {


    cursor = 0;

    write(PANTALLA_FD, screen_buffer+160,4000);
    cursor -= 80;


    return;
}

/***************************************************************
* check_screen_scroll
*
* 
* 
****************************************************************/
void check_screen_scroll() {
    if (cursor >= 2000) {
        page_roll();
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
    check_screen_scroll();
    
    /* ENTER */
    if (c == '\n') {
//         if (cursor == 2000 && vb_counter==160) {
//                 while (1) {
//                     int H;
//                     char * p = (char * ) 0xB8000;
//                     for (H=79; H<100;H++) {
//                             *(p+H*2)='0';
//                             *(p+H*2+1)='0';
//                     }
//                 }
//         }
        write(PANTALLA_FD, video_buffer, vb_counter);


//         if (cursor == 2000 && V_BUFFER_LENGTH-(cursor%80)*2==160) {
//                 while (1) {
//                     int H;
//                     char * p = (char * ) 0xB8000;
//                     for (H=79; H<100;H++) {
//                             *(p+H*2)='1';
//                             *(p+H*2+1)='0';
//                     }
//                 }
//         }
        write(PANTALLA_FD, clean_buffer, V_BUFFER_LENGTH-(cursor%80)*2 );
        vb_counter = 0;
        check_screen_scroll();
        return;
    }


    /* BACKSPACE */
    if (c == '\x08') {

        cursor -= 1;
        write(PANTALLA_FD, clean_buffer, 2);
        cursor -= 1;

        return;
    }
    
    /* OTHER CHARACTERS */
        
    if (! (vb_counter < V_BUFFER_LENGTH)) {

//         if (cursor == 2000 && vb_counter==160) {
//                 while (1) {
//                     int H;
//                     char * p = (char * ) 0xB8000;
//                     for (H=79; H<100;H++) {
//                             *(p+H*2)='3';
//                             *(p+H*2+1)='0';
//                     }
//                 }
//         }

        write(PANTALLA_FD, video_buffer, vb_counter);
        vb_counter = 0;
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
		offset = i + cursor*2;
		data = *((byte *)buffer+i);

// 		if (offset >= 4000) {
// 			while(1) {
// 					int H;
// 					char * p = (char * ) 0xB8000;
// 					for (H=0; H<2000;H++) {
// 					if(H<=3) {
// 						*(p+H*2)=digit(H,cursor)+'0';
// 			
// 						*(p+H*2+10)=digit(H,count)+'0';
// 					}
// 				}
// 			}
// 		}

		_int_80_caller(WRITE, fd, offset, data);
		screen_buffer[offset] = data;

	}
	cursor+=count/2;
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
	cursor = 0;
	write(PANTALLA_FD, blank_screen_buffer, 4000);
  	cursor = 0;
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
"                                                                                ",
"                                                                                "
};

void showSplashScreen() {
    int i;
    for (i = 0; i < 25; i++) {
        puts(splash_screen[i]);
    }

}
