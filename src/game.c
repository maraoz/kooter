#include "../include/stdio.h"
#include "../include/defs.h"
#include "../include/game.h"
#include "../include/util.h"
#include "../include/mouse.h"
#include "../include/video.h"



extern TTY tty[8];


/*
 * una matriz que almacena los elementos dentro del mapa
 */
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
char view[25][80][2];

/* sprites */

static pjT mario;
static object corazones[HEART_N];


/* 
 * main game function 
 * 
 * runs game main loop
 */
int game() {
    
    flush();
    cleanClipboard();
    hideMouseCursor();

    init_map_view();

    init_mario();
    init_hearts();

    updateView();
    showView();
    
    char c; 

    while ( (c = get_char()) != 'q') {

        int dx,dy;

        switch (c) {
            case(K_LEFT):
            case 'a':
                dx = -1;
                dy = 0;
                break;
            case(K_RIGHT):
            case 'd':
                dx = +1;
                dy = 0;
                break;
            case(K_UP):
            case 'w':
                dx = 0;
                dy = -1;
                break;
            case(K_DOWN):
            case 's':
                dx = 0;
                dy = +1;
                break;
            default:
                dx = 0;
                dy = 0;
                break;
        }

        if (mario.x+dx < 0) {
            dx = 79;
        }
        if (mario.x+dx >= 80) {
            dx = -79;
        }
        if (mario.y+dy < 0) {
            dy = +24;
        }
        if (mario.y+dy >= 25) {
            dy = -24;
        }

        int i;
        
        for (i=0; i< HEART_N; i++) {
            if ((mario.x+dx)+(mario.y+dy)*80 == corazones[i].x+corazones[i].y*80) {
                corazones[i].alive = 0;
            }
        }

        int flag = 1;
        for (i=0; i<HEART_N; i++) {
            if (corazones[i].alive)
                flag = 0;
        }

        if (can_move(dx,dy)) {
            
            mario.x += dx;
            mario.y += dy;
        } 


        if (dx != 0 || dy != 0) {
            updateView();
            showView();
        }


        if (flag) {
            k_clear_screen();
            int currentTTY = get_current_tty();
            tty[currentTTY].cursor = 1000;
            puts("FELICITACIONES, GANASTE!!!");
            flush();
            wait(10);
            
            break;
        }

    }

    return 0;
}

void updateView() {
    static int xBkp = 40;
    static int yBkp = 12;

    view[yBkp][xBkp][0] = ' ';
    view[yBkp][xBkp][1] = DEFAULT_TXT;

    int i;
    for (i=0; i<HEART_N;i++) {
        if (corazones[i].alive == 1) {
            view[corazones[i].y][corazones[i].x][0] = corazones[i].sprite;
            view[corazones[i].y][corazones[i].x][1] = RED_TXT;
        }
        else {
            view[corazones[i].y][corazones[i].x][0] = ' ';
            view[corazones[i].y][corazones[i].x][1] = DEFAULT_TXT;
        }
    }

    view[mario.y][mario.x][0] = mario.sprite;
    view[mario.y][mario.x][1] = YELLOW_TXT;


    yBkp = mario.y;
    xBkp = mario.x;

}

void showView(void) {
    int currentTTY = get_current_tty();
    tty[currentTTY].cursor = 0;
    check_offset('m', 4000);
    write(PANTALLA_FD, view, 4000);
    tty[currentTTY].cursor = 0;
}






void init_map_view(){
    int i,j;
    for (i=0; i<25;i++) {
        for (j=0; j<80;j++) {
            view[i][j][0] = map[i][j];
            view[i][j][1] = WHITE_TXT;
        }
    }
}


void init_mario() {
    mario.x = 40;
    mario.y = 12;
    mario.sprite = 1;
    mario.alive = 1;

}


void init_hearts() {

    static pointT start_coords[HEART_N] = {
        {0,1}, {11,0}, {5,15}, {1,23}, {23,23}, 
        {7,21}, {23,1}, {23,7}, {13,11}, {7,9},
        {21,13}, {5,5}, {23,19}, {21,5}, {3,3}
        
    };


    int i;
    for (i = 0; i< HEART_N; i++) {
        corazones[i].x = start_coords[i].x;
        corazones[i].y = start_coords[i].y;
        corazones[i].sprite = 3;
        corazones[i].alive = 1;
    }



    
}


int can_move(int dx, int dy) {

    return map[(mario.y+dy)][(mario.x+dx)] == ' ';

}


