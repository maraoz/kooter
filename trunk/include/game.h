
#ifndef _video_h_
#define _video_h_

#define K_UP 1
#define K_RIGHT 2
#define K_DOWN 3
#define K_LEFT 4

#define HEART_N 9

typedef struct object {
    char sprite;
    int x;
    int y;
} object;

typedef object pjT;

typedef struct pointT {
    int x;
    int y;
} pointT;

int game();

void updateView();

void showView();

void init_mario();

void init_map_view();

void init_hearts();

#endif
