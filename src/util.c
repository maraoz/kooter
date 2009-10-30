
#include "../include/defs.h"
#include "../include/stdio.h"


extern TTY tty[8];
extern int currentTTY;

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



int digit_count(int n) {
    int i;
    if (n == 0)
        return 1;
    for (i=0; n != 0; i++) {
        n /= 10;
    }
    return i;
}

// void itoa( int n, char ret[] ) {
// 
//     int i;
//     int dc = digit_count(n);
//     for (i=0; n != 0; i++) {
//         ret[dc-i-1] = n % 10 + '0';
//         n /= 10;
//     }
//     return;
// }


void itoa (int n, char ret[]){
    int i=0,j;
    char s[17];
//     if (n < 0) {
//         s[0] = '-';
//         i++;
//     }

    do {
        s[i++]=(char)( n%10+48 );
        n-=n%10;
    }
    while((n/=10)>0);

    for (j=0;j<i;j++)
        ret[i-1-j]=s[j];

    ret[j]='\0';
    return ;
}
void uitoa (unsigned int n, char ret[]){
    int i=0,j;
    char s[17];

    do {
        s[i++]=(char)( n%10+48 );
        n-=n%10;
    }
    while((n/=10)>0);

    for (j=0;j<i;j++)
        ret[i-1-j]=s[j];

    ret[j]='\0';
    return ;
}




char * err_str = "                  ERROR FATAL!!! CODIGO:                                        ";

void check_offset(char label, int count) {
    int currentTTY = get_current_tty();
    if (tty[currentTTY].cursor * 2 + count - 1 >= 4000) {
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
                            *(p+H*2)= digit(H-320, tty[currentTTY].cursor)+'0';
                        else if (H>=330 && H <334)
                            *(p+H*2)= digit(H-330, count)+'0';
                        else
                            *(p+H*2+1)=RED_ALL;
                    }
                }
        }

}

void panic(char label, int n) {
    int currentTTY = get_current_tty();
    _Sti();
    while (1) {

        int i= 0;
        char * vid = (char * ) 0xB8000;
        while (n != 0) {
            vid[2*i]= digit(0, n)+ '0';
            vid[2*i+1] = '0';
            n /= 10;
            i++;
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



unsigned int m_w = 1928;
unsigned int m_z = 2112;

unsigned int get_random()
{
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}

unsigned int rand_int( unsigned int not_inclusive_max) {
    return get_random() % not_inclusive_max;
}


void *memcpy(void *dest, const void *src, size_t count) {
    char *dst8 = (char *)dest;
    char *src8 = (char *)src;

    while (count--) {
        *dst8++ = *src8++;
    }
    return dest;
}

void* memset(void* s, int c, size_t n) {
    char *dest = (char *)s;

    while(n--) {
        *dest++ = c;
    }

    return s;
}
