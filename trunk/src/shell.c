#include "../include/defs.h"
#include "../include/stdio.h"
#include "../include/shell.h"

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
extern char * splash_screen[25];
extern int tTicks;
extern int cursor;

/*
** matriz de dos filas, en las que se van a guardar:
** en la primera el comando ingresado
** en la segunda el parametro, en caso de haberlo
*/
char data[2][LONG_STR];

/*
** vector en el que se guarda lo ingresado por el usuario
*/
char in[DIM_STR];

/*
** vector para guardar la pantalla durante el screen saver
*/
char bufferScr[TCIRC_SIZE*2];
/*
** variable con la posicion en el buffer
*/
int pos;

/*
** variable con el tiempo para que entre el screen saver
*/
int entraSp;

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
separaPorEspacios(char *s, char out[][LONG_STR])
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
llamaFunc(char s[2][LONG_STR])
{
// 	int rec;

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
		setTimeSp((atoi(s[1])));
		puts("tiempo seteado = ");
		puts(s[1]);
		puts(" segundos");
		return SETTIME_CD;
	}
	else if(str_cmp(s[0], "activaSp"))
	{
		read(PANTALLA_FD, bufferScr, 4000);
// 		rec=cursor;
		activaSp();
		write(PANTALLA_FD, bufferScr, 4000);
// 		cursor=rec;
		return ACTSP_CD;
	}
	else if(str_cmp(s[0], "turnOff"))
	{
// 		turnOff();
		return EXIT_CD;
	}
	else if(str_cmp(s[0], "garbage"))
	{
// 		garbage();
		return GBG_CD;
	}
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
	int ret=AUX;
	int i;
	int c;
	int rec;
	entraSp=10;
	tTicks=0;

	while(1)
	{

// 		if(tTicks>entraSp*18)
// 		{
// 			rec=cursor;
// 			read(PANTALLA_FD, bufferScr, 4000);
// 			activaSp();
// 			write(PANTALLA_FD, bufferScr, 4000);
// 			cursor=rec;
// 		}

		if(ret!=AUX && ret!=CLEAR_CD && ret!=NO_CD)
		{
			put_char('\n');
			flush();
		}

		

		print_nline();

		

		i=0;
		while((c=get_char())!='\n')
		{
			if(c!='\x08')
			{
				if(i<100)
					in[i++]=c;
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
		put_char(c);
		flush();
		in[i]=0;

		separaPorEspacios(in, data);

		ret=llamaFunc(data);

		data[0][0]=data[1][0]=0;
	}
}

void
activaSp()
{
	int i;
	interrupted=0;
	while(interrupted==0)
		for(i = 0; i < 25; i++)
		{
			if(interrupted!=0)
				break;
			puts(splash_screen[i]);
		}
	return;
}

void
setTimeSp(int time)
{
	entraSp=time;
}
