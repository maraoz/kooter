#include<stdio.h>
#include<stdlib.h>
/* Recibe un string y en base a eso, llama a una funcion */
/*
echo
imprimirbasura
salvapantallas
settimesp
*/

/* strcmp retorna 1 si los strings eran iguales y 0 en caso contrario */
/* necesita que ambos strings sean iguales hasta el /0 */
int
strcmp(char *s, char *t)
{
	int i;
	int flag=1;

	for(i=0; s[i] && t[i] && flag; i++)
		if( s[i] != t[i] )
			flag=0;

	return (s[i]==0 && t[i]==0);
}

/* separaPorEspacios toma el string s,
** s deberia ser 1 comando y 1 o 0 parametros,
** en caso de no tener parametros */
char **
separaPorEspacios(char *s, char **out)
{
	int i, j;

	out=malloc(2*sizeof(char*));
	out[0]=malloc(15);
	out[1]=malloc(15);

	while( *s == ' ' )
		s++;

	for(i=0, j=0; s[i] != ' ' && s[i] != 0; i++, j++)
		out[0][j] = s[i];
	out[0][j]=0;

	while( s[i] == ' ' )
		i++;

	for(j=0; s[i] != ' ' && s[i] != 0; i++, j++)
		out[1][j] = s[i];
	out[1][j]=0;

	return out;
}

int
main(void)
{
	char *s=" holas";
	char **impr;

	impr=separaPorEspacios(s, impr);

	printf("%s y %s\n", impr[0], impr[1]);
}
