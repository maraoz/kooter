
#ifndef _video_h_
#define _video_h_



typedef struct personaje {
    int x;
    int y;
} pjT;

int game();

void updateView( pjT mario);

void showView(void);

#endif
