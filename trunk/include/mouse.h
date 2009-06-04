#ifndef _mouse_h_
#define _mouse_h_

void leomouse (int b);

void updateMouse(void);

mouseSt mouse_now(void);

void paste(void);

void copy(point start, point end);

void hideMouseCursor(void);

void showMouseCursor(void);

void moveMouseCursor(point pto);

#endif
