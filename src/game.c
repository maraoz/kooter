#include "../include/stdio.h"
#include "../include/defs.h"
#include "../include/game.h"
#include "../include/util.h"


char view[4000];
extern int cursor;

pjT * mario;

int game() {
    
    flush();
    
    cursor = 0;
    init_mario(mario);
    
    updateView(mario);
    showView();
    
    byte c;
    while ( (c = get_char()) != 'q') {
        switch (c) {
            case('a'):
                mario->x--;
                if (mario->x < 0 ) {
                    mario->x = 79;
                    }
                break;
            case('d'):
                mario->x++;
                if (mario->x >= 80) {
                    mario->x = 0;
                    }
                break;
            case('w'):
                mario->y--;
                if (mario->y < 0) {
                    mario->y = 24;
                    }
                break;
            case('s'):
                mario->y++;
                if (mario->y >= 25) {
                    mario->y = 0;
                    }
                break;
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

