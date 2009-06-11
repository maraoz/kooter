#ifndef _video_h_
#define _video_h_
#include "../include/defs.h"	

void screenShow(byte c, int cursor_toWrite);

byte fetch(void);

void hideMouseCursor(void);

void showMouseCursor(int pos);

void showSelection(point start, point end);

void hideSelection(void);

#endif
