#include "../include/shell.h"
#include "../include/defs.h"
#include "../include/stdio.h"
#include "../include/process.h"
#include "../include/files.h"
#include "../include/scheduler.h"

extern int current_process;
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
    puts(" ");
    puts(argv[1]);
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
    putln("Esta versión de Kooter no soporta SalvaPantallas");
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
        c = (c+1)%50;
        put_char(c+'a');
    }
}

#define CYCLE 10000

int
print_a(int argc, char* argv[]) {
    int n=0;
    while(1) {
        n++;
        if(n%CYCLE == 0) put_char('a');
    }
}
int
print_b(int argc, char* argv[]) {
    int n=0;
    while(1) {
        n++;
        if(!n%CYCLE == 0) put_char('b');
    }
}

int print_char(int argc, char * argv[]) {

    char c;
    puts("Ingrese un caracter por favor: ");
    while((c = get_char()) == -1 || c == '\x08');
    int n=0;
    while(1) {
        n++;
        if(!n%CYCLE == 0) put_char(c);
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




int help(int argc, char * argv[]) {
    puts("Kooter kernel version 1.0");
    put_char('\n');
    put_char('\n');
    puts("echo [string arguments]");
    move_cursor_inline(40);
    puts("clear");
    put_char('\n');
    puts("setTimeSp [numeric argument]");
    move_cursor_inline(40);
    puts("activaSp");
    put_char('\n');
    puts("dispImg");
    move_cursor_inline(40);
    puts("garbage");
    put_char('\n');
    puts("mario");
    move_cursor_inline(40);
    puts("infinite");
    put_char('\n');
    puts("print_a");
    move_cursor_inline(40);
    puts("print_b");
    put_char('\n');
    puts("print_char");
    move_cursor_inline(40);
    puts("top");
    put_char('\n');
    puts("kill [pid]");
    move_cursor_inline(40);
    puts("help");
    put_char('\n');
    put_char('\n');
    puts("Files functions:");
    put_char('\n');
    puts("rm [filename]");
    put_char('\n');
    puts("rmdir [directory]");
    move_cursor_inline(40);
    puts("print [filename]");
    put_char('\n');
    puts("put [filename][string]");
    move_cursor_inline(40);
    puts("chname [old_filename][new_filename]");
    put_char('\n');
    puts("chnamedir [old_name][new_name]");
    move_cursor_inline(40);
    puts("open [filename]");
    put_char('\n');
    puts("cd [tag_name]");
    move_cursor_inline(40);
    puts("ls [tag(optional)]");
    put_char('\n');
    puts("mkdir [tag_name]");
    move_cursor_inline(40);
    puts("taglist");
    put_char('\n');
    puts("addtag [filename][tagname]");
    move_cursor_inline(40);
    puts("rmtag [filename][tagname]");
    put_char('\n');
    //             puts("tagslong");
    //             move_cursor_inline(40);
    puts("filetags [filename]");
    put_char('\n');
    puts("cwd");
    move_cursor_inline(40);
    puts("locate [filename]");
    put_char('\n');
    puts("lsall");
    put_char('\n');
}






/* 
** Funciones de Files..
*/


int
cat(int argc, char *argv[]){
    
}

int
rm(int argc, char *argv[]){
    closef(argv[0]);
}

int
print(int argc, char *argv[]){
    PAGE * data;
    data = (PAGE*)palloc(1);
    int i;
    fread(argv[0],(char*)data);
    puts((char*)data);
    putln("");
    pfree(data,1);
}

int
put(int argc, char *argv[]){
    fwrite(argv[0],argv[1]);
}

int
rmdir(int argc, char *argv[]){
    rmdire(argv[0]);
}

int
chnamedir(int argc, char *argv[]){
    renamedir(argv[0],argv[1]);
}

int
chname(int argc, char *argv[]){
    rename(argv[0],argv[1]);
}

int
open(int argc, char *argv[]){
    openf(argv[0]);
}

int
chdir(int argc, char *argv[]){
    chdird(argv[0]);
}

int
ls(int argc, char *argv[]){
    lsdir(argv[0]);
}

int
mkdir(int argc, char *argv[]){
    mkdird(argv[0]);
}

int
taglist(int argc, char *argv[]){
    tags();
}

int
addtag(int argc, char *argv[]){
    addtags(argv[0],argv[1]);
}

int
rmtag(int argc, char *argv[]){
    rmtags(argv[0],argv[1]);
}

// int
// tagslong(int argc, char *argv[]){
//     tagslongs();
// }

int
filetags(int argc, char *argv[]){
    filetagss(argv[0]);
}

int
whereami(int argc, char *argv[]){
    whereamii();
}

int
locate(int argc, char *argv[]){
    located(argv[0]);
}

int
wrapkill(int argc, char *argv[]){
    pid_t pid = atoi(argv[0]);
    kill(pid);
}

int
lsall(int argc, char *argv[]){
    lslongall();
}

int
crash(int argc, char * argv[]) {
    char * p = (char*) 0xA0000000;
    *p = 'a';
}
