#include "../include/shell.h"
#include "../include/defs.h"
#include "../include/stdio.h"
#include "../include/process.h"

extern int entraSp;
extern TTY tty[8];
extern byte blank_screen_buffer[4000];
extern char * splash_screen[24];
extern int interrupted;
extern char bufferScr[TCIRC_SIZE*2];
extern char * screenSaverImg[25];

/*
** funcion que imprime en pantalla el texo en su argumento.
*/

int 
echo(int argc, char* argv[]) {
    puts(argv[0]);
}

/*
** funcion que setea el tiempo que tarda en entrar el screen saver.
*/

int
setTimeSp(int argc, char* argv[]) {
    int time = atoi(argv[0]);
    if(time < 0) {
        puts("tiempo no valido");
        return;
    }
    puts("tiempo seteado = ");
    puts(argv[0]);
    puts(" segundos");
    entraSp = time;
}

/*
** funcion que borra la pantalla.
*/

int
k_clear_screen(int argc, char* argv[]) {
    int currentTTY = get_current_tty();

    tty[currentTTY].cursor = 0;
    check_offset('5',4000);
    write(PANTALLA_FD, blank_screen_buffer, 4000);

    tty[currentTTY].cursor = 0;
}

/*
** funcion que activa el protector de pantalla.
*/

int
activaSp(int argc, char* argv[]) {
    return;
}

/*
** funcion que muestra la imagen.
*/

int
dispImg(int argc, char* argv[]) {
    int currentTTY = get_current_tty();
    tty[currentTTY].cursor=0;
    
    int i;
    for (i = 0; i < 24; i++) {
        puts(splash_screen[i]);
    }
    
    tty[currentTTY].cursor=2000-80;
}

/*
** funcion que imprime caracteres random en pantalla.
*/

int
garbage(int argc, char* argv[]) {
    char c = 'a';
    int i;
    for (i = 0; i<2000; i++) {
        put_char(c++%50+'a');
        flush();
    }
}

/*
** basura infinita
*/

int
infinite(int argc, char* argv[]) {
    char c = 'a';
    int i;
    for (i = 0; ; i++) {
        put_char(c++%50+'a');
        flush();
    }
}
/*
** funcion que ejecuta el juego mario.
*/

int
mario(int argc, char* argv[]) {
    game();
    
    int currentTTY = get_current_tty();
    tty[currentTTY].cursor = 0;
    check_offset('5',4000);
    write(PANTALLA_FD, blank_screen_buffer, 4000);
    tty[currentTTY].cursor = 0;
}


/*
** funcion que corre el timer tick
** aumenta en 1 el valor actual de tTicks y se encarga
** de controlar comienzo y fin del screen saver
*/

int
check_screen_saver(int argc, char* argv[]) {
    static int firstTime = 1;
    static int cursorBkp = 0;
    
    static int thisLine = 0;

    tTicks++;

    if(tTicks>entraSp*18 && firstTime)
        interrupted=0;
    
    if (firstTime && interrupted==0)
    {
        int currentTTY = get_current_tty();
        hideMouseCursor();
        read(PANTALLA_FD, bufferScr, 4000);
        cursorBkp = tty[currentTTY].cursor;
        
        tty[currentTTY].cursor = 0;
        check_offset('5',4000);
        write(PANTALLA_FD, blank_screen_buffer, 4000);
        tty[currentTTY].cursor = 0;
        
        firstTime = 0;
        interrupted=0;
    }

    if (interrupted == 0)
        if (tTicks % 2)
        {
            if (thisLine == 25)
                thisLine = 0;
            puts(screenSaverImg[thisLine++]);
        }

    if (interrupted == 1 && firstTime == 0)
    {
            int currentTTY = get_current_tty();
            borra_buffer();
            tty[currentTTY].cursor = 0;
            check_offset('p',4000);
            write(PANTALLA_FD, bufferScr, 4000);
            tty[currentTTY].cursor = cursorBkp;
            tTicks = 0;
            firstTime = 1;
    }
}