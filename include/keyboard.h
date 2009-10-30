#ifndef _keyboard_h_
#define _keyboard_h_

void leoteclado (int k);

byte ktoa(int c);

byte next_char ();
int non_blocking_next_char();

void writeToKeyboard(byte c);

int isFs(int c);

int isFlechita(int c);

#define K_UP 1
#define K_RIGHT 2
#define K_DOWN 3
#define K_LEFT 4

#endif
