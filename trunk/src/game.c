#include "../include/stdio.h"
#include "../include/defs.h"
#include "../include/game.h"


char view[4000];
extern int cursor;

int game() {
    byte c;
    pjT mario;
    mario.x =  40;
    mario.y = 12;

    showView();
    while ( (c = get_char()) != 'q') {
        switch (c) {
            case('a'):
                mario.x--;
                break;
            case('d'):
                mario.x++;
                break;
            case('w'):
                mario.y--;
                break;
            case('s'):
                mario.y++;
                break;
        }

        updateView(mario);

        showView();
    }

    return 0;
}

void updateView( pjT mario) {
    static int xBkp = 40;
    static int yBkp = 12;

    view[(yBkp*80 + xBkp)*2] = ' ';
    view[(yBkp*80 + xBkp)*2 + 1 ] = DEFAULT_TXT;
    

    view[2*(mario.y*80+mario.x)] = 'o';
    view[2*(mario.y*80+mario.x) + 1] = RED_TXT;
}

void showView(void) {
    write(PANTALLA_FD, view, 4000);
    cursor = 0;
}


