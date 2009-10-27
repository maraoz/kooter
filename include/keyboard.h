#ifndef _keyboard_h_
#define _keyboard_h_

void leoteclado (int k);

byte ktoa(int c);

byte next_char ();
int non_blocking_next_char();

void writeToKeyboard(byte c);

int isFs(int c);

#endif
