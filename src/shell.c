#include "../include/defs.h"
#include "../include/stdio.h"
#include "../include/shell.h"
#include "../include/game.h"
#include "../include/files.h"
#include "../include/top.h"
#include "../include/process.h"
#include "../include/shell_proc.h"

#include "../include/scheduler.h"

#include "../include/kc.h"


/*
** Variables globales:
*/

/* variable que esta en 0 mientras no lleguen interrupciones */
extern int interrupted;
/* variable con la cantidad de timerTicks que llegaron desde el ultimo reinicio */
extern int tTicks;

extern pid_t current_process;

extern TTY tty[8];
extern int currentTTY; 
/*
** matriz de dos filas, en las que se van a guardar:
** en la primera el comando ingresado
** en la segunda el parametro, en caso de haberlo
*/
char data[3][LONG_STR_TKN];

/*
** vector en el que se guarda lo ingresado por el usuario
*/
char in[LONG_STR_CMD];

/*
** vector para guardar la pantalla durante el screen saver
*/
char bufferScr[TCIRC_SIZE*2];

/*
** variable con el tiempo para que entre el screen saver
*/
int entraSp=30;

/*
** guarda el retorno de la ultima funcion ejecutada
*/
int ret=AUX;

/*
** Imagen para el protector de pantalla
*/

boolean isBackground[8];

char * screenSaverImg[25] = {
"       x                                                                        ",
"                                                                          x     ",
"                                                    x                           ",
"                                                                                ",
"               x                                                                ",
"                                                                                ",
"                                                                                ",
"                                                                                ",
"                                                              x                 ",
"                         x                                                      ",
"                                                                                ",
"                                                                                ",
"                                                                                ",
"                                                                                ",
"      x                            x                                            ",
"                                                                                ",
"                                                                x               ",
"                                                                                ",
"                                                     x                          ",
"                                                                                ",
"                         x                                                      ",
"                                                                                ",
"                                                                 x              ",
"             x                                                                  ",
"   x                                                                            "
};


/*
** print_nline al ser llamada imprime el inicio de la linea de comandos.
*/

void
print_nline()
{
    puts("kooter > ");
    flush();
}




/* str_cmp retorna 1 si los strings eran iguales y 0 en caso contrario */
/* necesita que ambos strings sean iguales hasta el 0 */

int
str_cmp(char *s, char *t)
{
    int i;
    int flag=1;

    for(i=0; s[i] && t[i] && flag; i++)
        if( s[i] != t[i] )
            flag=0;

    return (s[i]==0 && t[i]==0 && flag);
}

/* str_cpy retorna la cantidad de bytes copiados */


int
str_cpy(char *s, char *t)
{
    int i;

    for(i=0; t[i]; i++)
        s[i]=t[i];
    s[i++]=0;
    return i-1;
}

int
strncpy(char *s, char *t, int n)
{
    int i;

    for(i=0; i<n && t[i]; i++)
        s[i]=t[i];
    s[i++]=0;
    return i-1;
}

/* str_len retorna la dimension de un string de char */

int
str_len(char *s)
{
    int i;

    for(i=0; s[i]; i++)
        ;

    return i;
}

/*
** recibe una cadena de numeros ascii y la transforma en un int
*/

int
atoi(char *s)
{
    int num=0;

    if(isdigit(*s)==0)
        return -1;

    while(isdigit(*s))
        num=num*10+(*(s++)-'0');

    if(num>10000 || num<=0 || *s!=0)
        num = -1;

    return num;
}

/*
** separaPorEspacios toma el string s,
** s deberia ser 1 comando y 1 o 0 parametros,
** en caso de no tener parametros devuelve solo el comando
** y no escribe en el parametro
*/

void
separaPorEspacios(char *s, char out[][LONG_STR_TKN])
{
    int i, j;
    int currentTTY = get_current_tty();
    /* out[0]= direccion para guardar comando */
    /* out[1]= direccion para guardar parametro */

    int ln = str_len(s);
    if(s[ln-1] == '&') {
        isBackground[currentTTY] = TRUE;
    } else {
        isBackground[currentTTY] = FALSE;
    }
    
    out[0][0] = out[1][0] = out[2][0] = 0;
    
    while( *s == ' ' )
        s++;

    for(i=0, j=0; s[i] != ' ' && s[i] != 0 && j<LONG_STR_TKN; i++, j++)
        out[0][j] = s[i];
    out[0][j]=0;

    while( s[i] == ' ' )
        i++;
    
    if( s[i] == 0 )
        return;

    for(j=0; s[i] != ' ' && s[i] != 0 && j<LONG_STR_TKN; i++, j++)
        out[1][j] = s[i];
    out[1][j]=0;
    
    while( s[i] == ' ' )
        i++;

    if( s[i] == 0 )
        return;

    for(j=0; s[i] != 0 && j<LONG_STR_TKN; i++, j++)
        out[2][j] = s[i];
    out[2][j]=0;
    
    while( s[i] == ' ' )
        i++;

    return;
}

/*
** llamaFunc recibe un comando y un parametro.
** y deriva en llamar a la funcion que corresponda
** en funcion del comando ingresado
*/

int
llamaFunc(char s[][LONG_STR_TKN])
{
    int currentTTY = get_current_tty();
    int cursorBkp;
    int arg_extra=0;

    char * str[3];
    str[0] = s[1];
    str[1] = s[2];
    str[2] = (char*)0;
    
    if(s[0][0]==0)
        return NO_CD;
    else if(str_cmp(s[0], "echo"))
    {

        create_process(echo, 1, 1, str, 1, 1, isBackground[currentTTY], currentTTY, current_process,"echo");
        if (!isBackground[currentTTY])
                wait_children();
        return ECHO_CD;
    }
    else if(str_cmp(s[0], "clear"))
    {
        if(s[1][0]==0)
        {
            create_process(k_clear_screen, 1, 1, str, 1, 1, isBackground[currentTTY], currentTTY, current_process,"clear_screen");
            if (!isBackground[currentTTY])
                wait_children();
            return CLEAR_CD;
        }
        else
            arg_extra=1;
    }
    else if(str_cmp(s[0], "setTimeSp"))
    {
        if(s[1][0]==0)
        {
            create_process(setTimeSp, 1, 1, str, 1, 1, isBackground[currentTTY], currentTTY, current_process,"setTimeSp");
            if (!isBackground[currentTTY])
                wait_children();
            return SETTIME_CD;
        }
        else
        {
            puts("Not enough arguments");
            return CODE_CD;
        }
    }
    else if(str_cmp(s[0], "activaSp"))
    {
        if(s[1][0]==0)
        {
            create_process(activaSp, 1, 1, str, 1, 1, isBackground[currentTTY], currentTTY, current_process,"activaSp");
            return ACTSP_CD;
            if (!isBackground[currentTTY])
                wait_children();
        }
        else
            arg_extra=1;
    }
    else if(str_cmp(s[0], "dispImg"))
    {
        if(s[1][0]==0)
        {
            create_process(dispImg, 1, 1, str, 1, 1, isBackground[currentTTY], currentTTY, current_process,"dispImg");
            if (!isBackground[currentTTY])
                wait_children();
            return DSPIMG_CD;
        }
        else
            arg_extra=1;
    }
    else if(str_cmp(s[0], "garbage"))
    {
        if(s[1][0]==0)
        {
            create_process(garbage, 1, 1, str, 1, 1, isBackground[currentTTY], currentTTY, current_process,"garbage");
            if (!isBackground[currentTTY])
                wait_children();
            return GBG_CD;
        }
        else
            arg_extra=1;
    }
    else if(str_cmp(s[0], "mario"))
    {
        if(s[1][0]==0)
        {
            create_process(mario, 1, 1, str, 1, 1, isBackground[currentTTY], currentTTY, current_process, "mario");
            if (!isBackground[currentTTY])
                wait_children();
            return MARIO_CD;
        }
        else
            arg_extra=1;
    }
    else if(str_cmp(s[0], "mkdir"))
    {
        if(s[1][0]==0)
        {
            puts("Not enough arguments");
            return CODE_CD;
        }
        else
        {
            create_process(mkdir,1,1,str,1,1,isBackground[currentTTY],currentTTY,current_process,"mkdir");
            if (! isBackground[currentTTY])
                wait_children();
            return MKDIR_CD;
        }
    }
    else if(str_cmp(s[0], "rm"))
    {
        if(s[1][0]==0)
        {
            puts("Not enough arguments");
            return CODE_CD;
        }
        else
        {
            create_process(rm,1,1,str,1,1,isBackground[currentTTY],currentTTY,current_process,"rm");
            if (! isBackground[currentTTY])
                wait_children();
            return RM_CD;
        }
    }
    else if(str_cmp(s[0], "print"))
    {
        if(s[1][0]==0)
        {
            puts("Not enough arguments");
            return CODE_CD;
        }
        else
        {
            create_process(print,1,1,str,1,1,isBackground[currentTTY],currentTTY,current_process,"print");
            if (! isBackground[currentTTY])
                wait_children();
            return PRINT_CD;
        }
    }
    else if(str_cmp(s[0], "put"))
    {
        if(s[1][0]==0 || s[2][0]==0)
        {
            puts("Not enough arguments");
            return CODE_CD;
        }
        else
        {
            create_process(put,1,1,str,1,1,isBackground[currentTTY],currentTTY,current_process,"put");
            if (! isBackground[currentTTY])
                wait_children();
            return PUT_CD;
        }
    }
    else if(str_cmp(s[0], "chdir"))
    {
        if(s[1][0]==0)
        {
            puts("Not enough arguments");
            return CODE_CD;
        }
        else
        {
            create_process(chdir,1,1,str,1,1,isBackground[currentTTY],currentTTY,current_process,"chdir");
            if (!isBackground[currentTTY]) wait_children();
            return CHDIR_CD;
        }
    }
    else if(str_cmp(s[0], "chname"))
    {
        if(s[1][0]==0 || s[2][0]==0)
        {
            puts("Not enough arguments");
            return CODE_CD;
        }
        else
        {
            create_process(chname,1,1,str,1,1,isBackground[currentTTY],currentTTY,current_process,"chname");
            if (!isBackground[currentTTY]) wait_children();
            return CHNAME_CD;
        }
    }
    else if(str_cmp(s[0], "rmdir"))
    {
        if(s[1][0]==0)
        {
            puts("Not enough arguments");
            return CODE_CD;
        }
        else
        {
            create_process(rmdir,1,1,str,1,1,isBackground[currentTTY],currentTTY,current_process,"rmdir");
            if (!isBackground[currentTTY]) wait_children();
            return RMDIR_CD;
        }
    }
    else if(str_cmp(s[0], "chnamedir"))
    {
        if(s[1][0]==0 || s[2][0]==0)
        {
            puts("Not enough arguments");
            return CODE_CD;
        }
        else
        {
            create_process(chnamedir,1,1,str,1,1,isBackground[currentTTY],currentTTY,current_process,"chnamedir");
            if (!isBackground[currentTTY]) wait_children();
            return CHNAMEDIR_CD;
        }
    }
    else if(str_cmp(s[0], "ls"))
    {
        create_process(ls,1,1,str,1,1,isBackground[currentTTY],currentTTY,current_process,"ls");
        if (!isBackground[currentTTY])
            wait_children();
        return LS_CD;
    }
    else if(str_cmp(s[0], "taglist"))
    {
        if(s[1][0]==0){
            create_process(taglist,1,1,(char**)0,1,1,isBackground[currentTTY],currentTTY,current_process, "taglist");
            if (!isBackground[currentTTY]) wait_children();
            return TAGLIST_CD;
        }
        else{
            arg_extra =1;
        }
    }
    else if(str_cmp(s[0], "rmdir"))
    {
        if(s[1][0]==0)
        {
            puts("Not enough arguments");
            return CODE_CD;
        }
        else
        {
            create_process(rmdir,1,1,str,1,1,isBackground[currentTTY],currentTTY,current_process,"rmdir");
            if (!isBackground[currentTTY]) wait_children();
            return RMDIR_CD;
        }
    }
    else if(str_cmp(s[0], "open"))
    {
        if(s[1][0]==0)
        {
            puts("Not enough arguments");
            return CODE_CD;
        }
        else
        {
            create_process(open,1,1,str,1,1,isBackground[currentTTY],currentTTY,current_process,"open");
            if (!isBackground[currentTTY]) wait_children();
            return OPEN_CD;
        }
    }
    else if(str_cmp(s[0], "top"))
    {
        create_process((int(*)(void))top,1,1,(char**)0,1,0,isBackground[currentTTY],currentTTY,current_process,"topaz");
        if (!isBackground[currentTTY])
                wait_children();
        return TOP_CD;
    }
    else if(str_cmp(s[0], "infinite"))
    {
        create_process(infinite,1,1,(char**)0,1,4,isBackground[currentTTY],currentTTY,current_process, "infinite");
        if (!isBackground[currentTTY]) wait_children();
        return INFINITE_CD;
    }
    else if(str_cmp(s[0], "help"))
    {
        if(s[1][0]==0)
        {
            puts("Kooter kernel version 1.0");
            put_char('\n');
            put_char('\n');
            puts("echo [string arguments]");
            put_char('\n');
            puts("clear");
            put_char('\n');
            puts("setTimeSp [numeric argument]");
            put_char('\n');
            puts("activaSp");
            put_char('\n');
            puts("dispImg");
            put_char('\n');
            puts("garbage");
            put_char('\n');
            puts("mario");
            put_char('\n');
            puts("infinite");
            put_char('\n');
            puts("top");
            put_char('\n');
            puts("help");
            put_char('\n');            
            put_char('\n');
            puts("Files functions:");
            put_char('\n');
            puts("rm [argument]");
            put_char('\n');
            puts("rmdir [argument]");
            put_char('\n');
            puts("print [argument]");
            put_char('\n');
            puts("put [argument1][argument2]");
            put_char('\n');
            puts("chname [argument1][argument2]");
            put_char('\n');
            puts("chnamedir [argument1][argument2]");
            put_char('\n');
            puts("open [argument]");
            put_char('\n');
            puts("chdir [argument]");
            put_char('\n');
            puts("ls [argument(optional)]");
            put_char('\n');
            puts("mkdir [argument]");
            put_char('\n');
            puts("taglist");
            put_char('\n');
             return CODE_CD;
        }
        else
            arg_extra=1;
    }

    if(arg_extra==0)
    {
        puts(s[0]);
        put_char(':');
        puts(" command not found");
        return CNF_CD;
    }
    else
    {
        puts(s[0]);
        put_char(':');
        puts(" does not receive arguments");
        return CNF_CD;
    }
}

/*
** ciclo principal del interprete de comandos
*/

void
shell()
{

    int i;
    int c;

    tTicks=0;
    flush();
    int currentTTY = get_current_tty();
    tty[currentTTY].cursor = 0;
    while(1)
    {
        if(ret==ECHO_CD || ret==CNF_CD || ret==SETTIME_CD || ret==GBG_CD || ret==CODE_CD)
            put_char('\n');

        print_nline();

        i=0;
        while((c=get_char())!='\n')
        {
            if(isFs(c)){
//                 flush();
//                 switch_tty(c&0x0F); /*le paso como parametro la terminal a la que quiero switchear */
//                 while(1);
            } else {
                if(c<0x05)
                    ;
                else if(c!='\x08')
                {
                    if(i<LONG_STR_CMD)
                        in[i]=c;
                    i++;
                    put_char(c);
                    flush();
                }
                else if(i>0)
                {
                    i--;
                    put_char(c);
                    flush();
                }
            }
        }
        put_char('\n');
        if(i>=LONG_STR_CMD)
            i=LONG_STR_CMD-1;
        in[i]=0;

        separaPorEspacios(in, data);

        ret=llamaFunc(data);

        data[0][0]=data[1][0]=0;
    }
}
