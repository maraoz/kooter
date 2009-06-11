#include "../include/defs.h"
#include "../include/stdio.h"
#include "../include/shell.h"
#include "../include/game.h"

/*
** Comandos que acepta el shell
**
** echo parametro, el cual imprime en pantalla parametro
** setTimeSp time, el cual setea en time el tiempo en el que se activa el protector de pantalla
** activaSp activa el protector de pantalla
** proximamente muchas mas!!!
**
*/


/*
** Variables globales:
*/

/*
** variable que esta en 0 mientras no lleguen interrupciones
*/
extern int interrupted;
extern int tTicks;
extern int cursor;

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
** es 1 mientras este el scr saver
*/
int scrIs=0;

/*
** guarda el retorno de la ultima funcion ejecutada
*/
int ret=AUX;


char * screenSaverImg[25] = {
"      x                                                                         ",
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

/* strcmp retorna 1 si los strings eran iguales y 0 en caso contrario */
/* necesita que ambos strings sean iguales hasta el 0 */

int
str_cmp(char *s, char *t)
{
	int i;
	int flag=1;

	for(i=0; s[i] && t[i] && flag; i++)
		if( s[i] != t[i] )
			flag=0;

	return (s[i]==0 && t[i]==0);
}

/*
** recibe una cadena de numeros ascii y la transforma en un int
*/

int
atoi(char *s)
{
	int num=0;

	while(isdigit(*s))
		num=num*10+(*(s++)-'0');

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

	for(i=0, j=0; s[i] != ' ' && s[i] != 0; i++, j++)
		out[0][j] = s[i];
	out[0][j]=0;

	while( s[i] == ' ' )
		i++;

	if( s[i] == 0 )
		return;

	for(j=0; s[i] != 0; i++, j++)
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
		k_clear_screen();
		return CLEAR_CD;
	}
	else if(str_cmp(s[0], "setTimeSp"))
	{
		if(atoi(s[1])==0)
			return SETTIME_CD;
		setTimeSp((atoi(s[1])));
		puts("tiempo seteado = ");
		puts(s[1]);
		puts(" segundos");
		return SETTIME_CD;
	}
	else if(str_cmp(s[0], "activaSp"))
	{
// 		read(PANTALLA_FD, bufferScr, 4000);
// 		cursorBkp=cursor;
//                 cursor = 0;
// 		activaSp();
// 		interrupted = 0;
// 		
// 		borra_buffer();
//                 cursor=0;
// 		write(PANTALLA_FD, bufferScr, 4000);
// 		cursor=cursorBkp;
                tTicks = entraSp * 18 +1;
		return ACTSP_CD;
	}
	else if(str_cmp(s[0], "dispImg"))
	{
		cursor=0;
		showSplashScreen();
		cursor=0;
		return DSPIMG_CD;
	}
	else if(str_cmp(s[0], "garbage"))
	{
		garbage();
		return GBG_CD;
	}
        else if(str_cmp(s[0], "mario"))
	{
		game();
		return MARIO_CD;
	}
	else
	{
		puts(s[0]);
		put_char(':');
		puts(" command not found");
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
	int rec;
	tTicks=0;

	
	flush();
	cursor = 0;

	while(1)
	{
		if(ret==ECHO_CD || ret==CNF_CD || ret==SETTIME_CD || ret==GBG_CD)
		{
			put_char('\n');
// 			flush();
		}

		print_nline();

		i=0;
		while((c=get_char())!='\n')
		{
			if(c!='\x08')
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
		put_char('\n');
		in[i]=0;

		separaPorEspacios(in, data);

		ret=llamaFunc(data);

		data[0][0]=data[1][0]=0;
	}
}

// void
// activaSp()
// {
// 	int i=0;
// 	
// 	k_clear_screen();
// 
// 	interrupted=0;
// 	while(interrupted==0)
// 		while(i < 25)
// 		{
// 			tTicks=0;
// 			if(interrupted!=0)
// 				break;
// // 			puts(screenSaverImg[i++]);
//                         if (i == 25) {
//                             i=0;
//                         }
// 		}
// 	flush();
// 	return;
// }

void
setTimeSp(int time)
{
	entraSp=time;
}

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


void check_screen_saver() {
	static int firstTime = 1;
	static int cursorBkp = 0;
	
	static int thisLine = 0;

	tTicks++;

	if(tTicks>entraSp*18 && firstTime)
		interrupted=0;
	
	if (firstTime && interrupted==0)
	{
		read(PANTALLA_FD, bufferScr, 4000);
                
		cursorBkp = cursor;
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
//                         put_char('\n');
		}

	if (interrupted == 1 && firstTime == 0)
	{
		cursor = 0;
		write(PANTALLA_FD, bufferScr, 4000);
                
		cursor = cursorBkp;
		tTicks = 0;
		firstTime = 1;
                borra_buffer();
                
                
                
	}
}
