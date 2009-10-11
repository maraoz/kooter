#include "../include/defs.h"
#include "../include/stdio.h"
#include "../include/shell.h"
#include "../include/game.h"

/*
** Variables globales:
*/

/* variable que esta en 0 mientras no lleguen interrupciones */
extern int interrupted;
/* variable con la cantidad de timerTicks que llegaron desde el ultimo reinicio */
extern int tTicks;


extern TTY tty[8];
extern int currentTTY;
extern int focusedTTY; 
/*
** matriz de dos filas, en las que se van a guardar:
** en la primera el comando ingresado
** en la segunda el parametro, en caso de haberlo
*/
char data[2][LONG_STR_TKN];

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

int
switch_tty(int new_tty){
    focusedTTY=new_tty;
    update_screen(); /*cambia lo que se esta viendo */
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

	/* out[0]= direccion para guardar comando */
	/* out[1]= direccion para guardar parametro */

	while( *s == ' ' )
		s++;

	for(i=0, j=0; s[i] != ' ' && s[i] != 0 && j<LONG_STR_TKN; i++, j++)
		out[0][j] = s[i];
	out[0][j]=0;

	while( s[i] == ' ' )
		i++;

	if( s[i] == 0 )
		return;

	for(j=0; s[i] != 0 && j<LONG_STR_TKN; i++, j++)
		out[1][j] = s[i];
	out[1][j]=0;

	return;
}

/*
** llamaFunc recibe un comando y un parametro.
** y deriva en llamar a la funcion que corresponda
** en funcion del comando ingresado
*/

int
llamaFunc(char s[2][LONG_STR_TKN])
{
	int cursorBkp;
	int arg_extra=0;

	if(s[0][0]==0)
		return NO_CD;
	else if(str_cmp(s[0], "echo"))
	{
		puts(s[1]);
		flush();
		return ECHO_CD;
	}
	else if(str_cmp(s[0], "clear"))
	{
		if(s[1][0]==0)
		{
			k_clear_screen();
			return CLEAR_CD;
		}
		else
			arg_extra=1;
	}
	else if(str_cmp(s[0], "setTimeSp"))
	{
		if(atoi(s[1])<0)
		{
			puts("tiempo no valido");
			return SETTIME_CD;
		}
		setTimeSp((atoi(s[1])));
		puts("tiempo seteado = ");
		puts(s[1]);
		puts(" segundos");
		return SETTIME_CD;
	}
	else if(str_cmp(s[0], "activaSp"))
	{
		if(s[1][0]==0)
		{
        	        tTicks = entraSp * 18 +1;
			return ACTSP_CD;
		}
		else
			arg_extra=1;
	}
	else if(str_cmp(s[0], "dispImg"))
	{
		if(s[1][0]==0)
		{
			tty[currentTTY].cursor=0;
			showSplashScreen();
			tty[currentTTY].cursor=2000-80;
			return DSPIMG_CD;
		}
		else
			arg_extra=1;
	}
	else if(str_cmp(s[0], "garbage"))
	{
		if(s[1][0]==0)
		{
			garbage();
			return GBG_CD;
		}
		else
			arg_extra=1;
	}
	else if(str_cmp(s[0], "mario"))
	{
		if(s[1][0]==0)
		{
			
			game(); 
                        k_clear_screen();
			return MARIO_CD;
		}
		else
			arg_extra=1;
	}
	else if(str_cmp(s[0], "uname"))
	{
		if(s[1][0]==0)
		{
			puts("Kooter v 1.0");
			flush();
			return CODE_CD;
		}
		else
			arg_extra=1;
	}
	else if(str_cmp(s[0], "pwd"))
	{
		if(s[1][0]==0)
		{
			puts("/");
			flush();
			return CODE_CD;
		}
		else
			arg_extra=1;
	}
	else if(str_cmp(s[0], "ls"))
	{
		if(s[1][0]==0)
		{
			puts("dev      root     home");
			put_char('\n');
			puts("bin      mnt      boot" );
			put_char('\n');
			puts("usr      etc      media");
			return CODE_CD;
		}
		else
			arg_extra=1;
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
			puts("uname");
			put_char('\n');
			puts("pwd");
			put_char('\n');
			puts("ls");
			put_char('\n');
			puts("help");
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
		flush();
		return CNF_CD;
	}
	else
	{
		puts(s[0]);
		put_char(':');
		puts(" does not receive arguments");
		flush();
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
	tty[currentTTY].cursor = 0;

	while(1)
	{
		if(ret==ECHO_CD || ret==CNF_CD || ret==SETTIME_CD || ret==GBG_CD || ret==CODE_CD)
			put_char('\n');

		print_nline();

		i=0;
		while((c=get_char())!='\n')
		{
            if(!isNotFs(c)){
                flush();
                switch_tty(c&0x0F); /*le paso como parametro la terminal a la que quiero switchear */
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

/*
** funcion que setea el tiempo que tarda en entrar el screen saver
*/

void
setTimeSp(int time)
{
	entraSp=time;
}

/*
** funcion que imprime basura, para testear el scroll de la pantalla
*/

void
garbage()
{
	char c = 'a';
        int i;
        for (i = 0; i<2000; i++)
        {
		put_char(c++%50+'a');
		flush();
	}
}

/*
** funcion que corre el timer tick
** aumenta en 1 el valor actual de tTicks y se encarga
** de controlar comienzo y fin del screen saver
*/

void check_screen_saver()
{
	static int firstTime = 1;
	static int cursorBkp = 0;
	
	static int thisLine = 0;

	tTicks++;

	if(tTicks>entraSp*18 && firstTime)
		interrupted=0;
	
	if (firstTime && interrupted==0)
	{
		hideMouseCursor();
		read(PANTALLA_FD, bufferScr, 4000);
		cursorBkp = tty[currentTTY].cursor;
		k_clear_screen();
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
                borra_buffer();
                tty[currentTTY].cursor = 0;
	        check_offset('p',4000);
	        write(PANTALLA_FD, bufferScr, 4000);
        	tty[currentTTY].cursor = cursorBkp;
        	tTicks = 0;
        	firstTime = 1;
	}
}
