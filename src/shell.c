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
** print_nline al ser llamada va una linea hacia abajo
** en donde imprime el inicio de la linea de comandos.
*/

void
print_nline()
{
	put_char('\n');
	flush();
	puts("kooter > ");
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

void
llamaFunc(char s[2][LONG_STR])
{
	if(str_cmp(s[0], "echo"))
		puts(s[1]);
	else if(str_cmp(s[0], "setTimeSp"))
	{
		/* llamo a la funcion que setea en que 
			tiempo se activa el protector de pantalla, le paso s[1] */
	}
	else if(str_cmp(s[0], "activaSp"))
	{
		/* llamo a la funcion, no recibe parametros */
	}
	else if(str_cmp(s[0], "exitpc"))
	{
		/* llamo a la funcion, no recibe parametros */
	}
	else
	{
		puts("command not found");
	}
}

/*
** ciclo principal del interprete de comandos
*/

void
shell()
{
	while(1)
	{
		print_nline();

		gets(in);

		separaPorEspacios(in, data);

		llamaFunc(data);
	}
}
