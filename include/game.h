
#ifndef _video_h_
#define _video_h_



typedef struct personaje {
    char sprite;
    int x;
    int y;
} pjT;

int game();

void updateView( pjT * mario);

void showView(void);

void init_mario(pjT * mario);

void init_map_view();

#endif
