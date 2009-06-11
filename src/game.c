#include "../include/stdio.h"
#include "../include/defs.h"
#include "../include/game.h"
#include "../include/util.h"

extern int cursor;

char view[4000];

char map[25][80] = {
    
    "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\x20\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB                                                       ",
    "\x20\x20\xDB\x20\x20\x20\xDB\x20\x20\x20\x20\x20\x20\x20\xDB\x20\x20\x20\xDB\x20\x20\x20\xDB\x20\xDB                                                       ",
    "\xDB\x20\xDB\x20\xDB\x20\xDB\x20\xDB\xDB\xDB\xDB\xDB\xDB\xDB\x20\xDB\x20\xDB\xDB\xDB\x20\xDB\x20\xDB                                                       ",
    "\xDB\x20\xDB\x20\xDB\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\xDB\x20\x20\x20\x20\x20\x20\x20\xDB                                                       ",
    "\xDB\x20\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\x20\xDB\xDB\xDB\xDB\xDB\xDB\xDB\x20\xDB\xDB\xDB\x20\xDB                                                       ",
    "\xDB\x20\xDB\x20\xDB\x20\x20\x20\xDB\x20\x20\x20\x20\x20\xDB\x20\xDB\x20\x20\x20\x20\x20\xDB\x20\xDB                                                       ",
    "\xDB\x20\xDB\x20\xDB\xDB\xDB\x20\xDB\x20\xDB\xDB\xDB\xDB\xDB\x20\xDB\x20\xDB\xDB\xDB\xDB\xDB\xDB\xDB                                                       ",
    "\xDB\x20\x20\x20\x20\x20\x20\x20\xDB\x20\x20\x20\x20\x20\xDB\x20\x20\x20\x20\x20\xDB\x20\x20\x20\xDB                                                       ",
    "\xDB\xDB\xDB\xDB\xDB\x20\xDB\xDB\xDB\x20\xDB\x20\xDB\xDB\xDB\x20\xDB\x20\xDB\xDB\xDB\x20\xDB\xDB\xDB                                                       ",
    "\xDB\x20\xDB\x20\x20\x20\x20\x20\xDB\x20\xDB\x20\xDB\x20\x20\x20\xDB\x20\x20\x20\x20\x20\xDB\x20\xDB                                                       ",
    "\xDB\x20\xDB\x20\xDB\xDB\xDB\xDB\xDB\x20\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\x20\xDB\x20\xDB                                                       ",
    "\xDB\x20\xDB\x20\x20\x20\x20\x20\x20\x20\x20\x20\xDB\x20\xDB\x20\x20\x20\xDB\x20\x20\x20\x20\x20\xDB                                                       ",
    "\xDB\x20\xDB\x20\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\x20\xDB\x20\xDB\xDB\xDB\xDB\xDB\xDB\xDB\x20\xDB                                                       ",
    "\xDB\x20\x20\x20\x20\x20\xDB\x20\x20\x20\x20\x20\x20\x20\xDB\x20\x20\x20\xDB\x20\xDB\x20\xDB\x20\xDB                                                       ",
    "\xDB\x20\xDB\xDB\xDB\xDB\xDB\x20\xDB\xDB\xDB\xDB\xDB\x20\xDB\xDB\xDB\x20\xDB\x20\xDB\x20\xDB\x20\xDB                                                       ",
    "\xDB\x20\x20\x20\xDB\x20\x20\x20\xDB\x20\x20\x20\x20\x20\xDB\x20\xDB\x20\x20\x20\xDB\x20\x20\x20\xDB                                                       ",
    "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\x20\xDB\x20\xDB\xDB\xDB\x20\xDB\xDB\xDB\x20\xDB\x20\xDB\xDB\xDB                                                       ",
    "\xDB\x20\x20\x20\x20\x20\xDB\x20\x20\x20\xDB\x20\x20\x20\x20\x20\xDB\x20\x20\x20\x20\x20\x20\x20\xDB                                                       ",
    "\xDB\xDB\xDB\x20\xDB\x20\xDB\xDB\xDB\x20\xDB\xDB\xDB\x20\xDB\x20\xDB\x20\xDB\xDB\xDB\xDB\xDB\xDB\xDB                                                       ",
    "\xDB\x20\x20\x20\xDB\x20\x20\x20\x20\x20\xDB\x20\x20\x20\xDB\x20\xDB\x20\x20\x20\x20\x20\x20\x20\xDB                                                       ",
    "\xDB\x20\xDB\x20\xDB\x20\xDB\xDB\xDB\xDB\xDB\xDB\xDB\x20\xDB\x20\xDB\x20\xDB\xDB\xDB\xDB\xDB\xDB\xDB                                                       ",
    "\xDB\x20\xDB\x20\xDB\x20\xDB\x20\x20\x20\x20\x20\x20\x20\xDB\x20\x20\x20\x20\x20\x20\x20\xDB\x20\x20                                                       ",
    "\xDB\xDB\xDB\x20\xDB\x20\xDB\xDB\xDB\x20\xDB\xDB\xDB\x20\xDB\xDB\xDB\x20\xDB\x20\xDB\x20\xDB\x20\xDB                                                       ",
    "\xDB\x20\x20\x20\xDB\x20\xDB\x20\x20\x20\xDB\x20\x20\x20\x20\x20\xDB\x20\xDB\x20\xDB\x20\x20\x20\xDB                                                       ",
    "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB                                                       "
};

extern int cursor;

pjT * mario;

int game() {
    
    flush();



    init_map_view();
    init_mario(mario);
    
    updateView(mario);
    showView();
    
    char c; 

    while ( (c = get_char()) != 'q') {

        int dx,dy;

        switch (c) {
            case('a'):
                dx = -1;
                dy = 0;
                break;
            case('d'):
                dx = +1;
                dy = 0;
                break;
            case('w'):
                dx = 0;
                dy = -1;
                break;
            case('s'):
                dx = 0;
                dy = +1;
                break;
            default:
                dx=0;
                dy=0;
                break;
        }

        if (mario->x+dx < 0) {
            dx = 79;
        }
        if (mario->x+dx >= 80) {
            dx = -79;
        }
        if (mario->y+dy < 0) {
            dy = +24;
        }
        if (mario->y+dy >= 25) {
            dy = -24;
        }



        if (can_move(dx,dy)) {
            
            mario->x += dx;
            mario->y += dy;
        } 
        

        updateView(mario);
        showView();
    }

    return 0;
}

void updateView( pjT * mario) {
    static int xBkp = 40;
    static int yBkp = 12;

    view[(yBkp*80 + xBkp)*2] = ' ';
    view[(yBkp*80 + xBkp)*2 + 1 ] = DEFAULT_TXT;
    

    view[2*(mario->y*80+mario->x)] = mario->sprite;
    view[2*(mario->y*80+mario->x) + 1] = RED_TXT;
    
    yBkp = mario->y;
    xBkp = mario->x;
    
}

void showView(void) {

    cursor = 0;
    check_offset('m', 4000);
    write(PANTALLA_FD, view, 4000);
    cursor = 0;
}

void init_mario(pjT * mario) {
    
    mario->x = 40;
    mario->y = 12;
    mario->sprite = 1;
}

void init_map_view(){
    int i,j;
    for (i=0; i<25;i++) {
        for (j=0; j<80;j++) {
            view[(i*80+j)*2] = map[i][j];
            view[(i*80+j)*2+1] = WHITE_TXT;
        }
    }
}

int can_move(int dx, int dy) {

    return map[(mario->y+dy)][(mario->x+dx)] == ' ';

}


