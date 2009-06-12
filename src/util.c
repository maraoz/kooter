
#include "../include/stdio.h"


extern int cursor;

/* En este archivo se encuentran funciones varias que sirven de utilitarios */

int isalpha(int c){
    return (('a' <= c &&  c <= 'z' )||('A' <= c && c <= 'Z'));
}

int isdigit(int c){
    return ('0' <= c && c <= '9');
}

int isalphanum(int c){
  return isdigit(c) && isalpha(c);
}

int abs(int a){
    return (a>0)?a:-a;
}

int max(int a, int b){
    return (a>b)?a:b;
}

int min(int a, int b){
    return (a<b)?a:b;
}

void wait(int t) {
    int i,j;
    int a;

    for (i=0; i<t*100;i++) {
        for (j=0; j<t*100;j++) {
           a <<= 1; 
           a += a-- + 0xfb0c28c3;
           a = a >> 4;
        }
    }
}

void itoa(int num, char v[]){
	
	if(num == 0)
		return;
	else
		itoa(num/10, v+1);
		v[0]=num%10+'0';
}





char * err_str = "                  ERROR FATAL!!! CODIGO:                                        ";

void check_offset(char label, int count) {
    
    if (cursor*2 +count-1 >= 4000) {
                while (1) {
                    int H;
                    char * p = (char * ) 0xB8000;
                    for (H=0; H<2000;H++) {
                        if (H>=0 && H <160)
                            *(p+H*2+1)=RED_ALL;
                        else if (H>=160 && H <240)
                            *(p+H*2)=err_str[(H-160)];
                        else if (H>=240 && H <320)
                            *(p+H*2)=label;
                        else if (H>=320 && H <324)
                            *(p+H*2)= digit(H-320, cursor)+'0';
                        else if (H>=330 && H <334)
                            *(p+H*2)= digit(H-330, count)+'0';
                        else
                            *(p+H*2+1)=RED_ALL;
                            
                    }
                }
        }

}


int digit(int indice, int numero) {
    int var = 0;
    while (var != indice ) {
        var++;
        numero /= 10;
    }
    return numero % 10;
}




