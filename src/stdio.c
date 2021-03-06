
#include "../include/defs.h"
#include "../include/stdio.h"
#include "../include/util.h"
#include "../include/queue.h"


// bloque de control de procesos
extern context_t bcp[];
extern pid_t current_process;


TTY tty[8];
int focusedTTY = 0;

queue_t tty_kb_queues[8];

/****************************************************************
* init_ttys
* 
* inicializa las terminales y sus estructuras de control
*
****************************************************************/
void init_ttys() {
    int i,j;

    for(i=0;i<8;i++) {
        tty[i].kb_buffer = &tty_kb_queues[i];
        queue_init(tty[i].kb_buffer);
        tty[i].cursor = 0;
        for(j=0;j<4160;j++){
            tty[i].view[j]=0;
        }
    }
}

int
switch_tty(int new_tty){
    focusedTTY=new_tty;
    update_screen();
}

void move_cursor_to(int pos) {
    if (0 <= pos && pos < 2000) {
        int currentTTY = get_current_tty();
        tty[currentTTY].cursor = pos;
    }
}

void go_to_line_start(){
    int currentTTY = get_current_tty();
    int offset = tty[currentTTY].cursor % 80;
    tty[currentTTY].cursor -= offset;
}

void move_cursor_inline(int offset) {
    if (0 <= offset && offset < 80) {
        int currentTTY = get_current_tty();
        go_to_line_start();
        tty[currentTTY].cursor += offset;
    }
}




/****************************************************************
* update_screen
*
* muestra la pantalla entera de la terminal
*
****************************************************************/
void update_screen() {
    int bkp_cursor = tty[focusedTTY].cursor;
    tty[focusedTTY].cursor = 0;
    write2(PANTALLA_FD, tty[focusedTTY].view,4000);
    tty[focusedTTY].cursor = bkp_cursor;

}




size_t write2(int fd, const void* buffer, size_t count) {
    int i;
    int offset;
    byte data;

    for ( i = 0 ; i<count; i++) {
        offset = i + tty[focusedTTY].cursor*2;
        data = *((byte *)buffer+i);

        _int_80_caller(WRITE, fd, offset, data);

    }
    tty[focusedTTY].cursor+=count/2;

    return 0;
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

byte blank_screen_buffer[4000] = {' ', DEFAULT_TXT, ' ', DEFAULT_TXT, ' ', DEFAULT_TXT};

void page_roll(int backwards) {
    int currentTTY = get_current_tty();

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
    int currentTTY = get_current_tty();
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
#define V_BUFFER_LENGTH 360


byte clean_buffer[8][V_BUFFER_LENGTH] = {0};


void put_char( byte c) {
    check_screen_scroll(0);
    int currentTTY = get_current_tty();

    /* ENTER */
    if (c == '\n') {

        check_offset('2',160-(tty[currentTTY].cursor%80)*2);
        write(PANTALLA_FD, clean_buffer, 160-(tty[currentTTY].cursor%80)*2 );
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

    if (tty[currentTTY].cursor >= 2000) {
        check_screen_scroll(1);
    }

    char temp[2];
    temp[0] = c;
    temp[1] = DEFAULT_TXT ;
    write(PANTALLA_FD, temp, 2);

}


/***************************************************************
* get_char
*
* Lee un caracter en pantalla y lo devuelve
* 
****************************************************************/
#define K_BUFFER_LENGTH 1

byte keyboard_buffer[8][K_BUFFER_LENGTH] = {{0}};
int kb_counter[8] = {K_BUFFER_LENGTH};
int n_read[8] = {K_BUFFER_LENGTH};

byte get_char() {
    int currentTTY = get_current_tty();
    while (read(TECLADO_FD, keyboard_buffer[currentTTY], 1) == 0);
    return keyboard_buffer[currentTTY][0];

}

/***************************************************************
* flush
*
* si queda algo en el buffer de put_char, lo imprime
* 
****************************************************************/
void flush() {

//    int currentTTY = get_current_tty();
//     if (tty[currentTTY].cursor + vb_counter[currentTTY] >= 2000) {
// 
//         check_screen_scroll(vb_counter[currentTTY]);
//         check_offset('9',vb_counter[currentTTY]);
// 
//         write(PANTALLA_FD, video_buffer[currentTTY], vb_counter[currentTTY]);
//         vb_counter[currentTTY] = 0;
//     } else {
//         check_offset('4',vb_counter[currentTTY]);
//         write(PANTALLA_FD, video_buffer[currentTTY], vb_counter[currentTTY]);
//         vb_counter[currentTTY] = 0;
//     }

}

void borra_buffer() {
    int currentTTY = get_current_tty();
    return;
}





/***************************************************************
* write
*
* Escribe count caracteres copiandolos de buffer al dispositivo
* descripto por el file descriptor fd. Llama a la int80h
*
* Recibe como parametros:
* - File Descriptor
* - Buffer del source
* - Cantidad
****************************************************************/

size_t write(int fd, const void* buffer, size_t count) {
    int i;
    int offset;
    byte data;

    int currentTTY = get_current_tty();
    if (bcp[current_process].process.background == FALSE) {
        for ( i = 0 ; i<count; i++) {
            offset = i + tty[currentTTY].cursor*2;
            data = *((byte *)buffer+i);

        
            if (currentTTY == focusedTTY) {
                _int_80_caller(WRITE, fd, offset, data);
            }
            tty[currentTTY].view[offset] = data;
        }
        tty[currentTTY].cursor+=count/2;
    }
    
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
    int currentTTY = get_current_tty();
    _Cli();
    for ( i = 0 ; i<count; i++) {
        b[i] = _int_80_caller(READ, fd, i, 0);
    }
    _Sti();
    return i;
}

char * BLANK_LINE = "                                                                                ";

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

