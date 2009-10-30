#include "../include/stdio.h"
#include "../include/defs.h"
#include "../include/game.h"
#include "../include/util.h"
#include "../include/mouse.h"
#include "../include/keyboard.h"
#include "../include/video.h"



extern TTY tty[8];
extern int mario_is_playing;




/* 
 * main game function 
 * 
 * runs game main loop
 */
int game() {


    

    game_t my_game_obj = {{
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
    },{},{},{}};

    game_t * my_game = &my_game_obj;
    flush();
    cleanClipboard();
    hideMouseCursor();

    init_map_view(my_game);

    init_mario(my_game);
    init_hearts(my_game);

    updateView(my_game);
    showView(my_game);
    
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

        if (my_game->mario.x+dx < 0) {
            dx = 79;
        }
        if (my_game->mario.x+dx >= 80) {
            dx = -79;
        }
        if (my_game->mario.y+dy < 0) {
            dy = +24;
        }
        if (my_game->mario.y+dy >= 25) {
            dy = -24;
        }

        int i;
        
        for (i=0; i< HEART_N; i++) {
            if ((my_game->mario.x+dx)+(my_game->mario.y+dy)*80 == my_game->corazones[i].x+my_game->corazones[i].y*80) {
                my_game->corazones[i].alive = 0;
            }
        }

        int flag = 1;
        for (i=0; i<HEART_N; i++) {
            if (my_game->corazones[i].alive)
                flag = 0;
        }

        if (can_move(my_game,dx,dy)) {
            
            my_game->mario.x += dx;
            my_game->mario.y += dy;
        } 


        if (dx != 0 || dy != 0) {
            updateView(my_game);
            showView(my_game);
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

    mario_is_playing = FALSE;

    return 0;
}

void updateView(game_t * my_game) {
    static int xBkp = 40;
    static int yBkp = 12;

    my_game->view[yBkp][xBkp][0] = ' ';
    my_game->view[yBkp][xBkp][1] = DEFAULT_TXT;

    int i;
    for (i=0; i<HEART_N;i++) {
        if (my_game->corazones[i].alive == 1) {
            my_game->view[my_game->corazones[i].y][my_game->corazones[i].x][0] = my_game->corazones[i].sprite;
            my_game->view[my_game->corazones[i].y][my_game->corazones[i].x][1] = RED_TXT;
        }
        else {
            my_game->view[my_game->corazones[i].y][my_game->corazones[i].x][0] = ' ';
            my_game->view[my_game->corazones[i].y][my_game->corazones[i].x][1] = DEFAULT_TXT;
        }
    }

    my_game->view[my_game->mario.y][my_game->mario.x][0] = my_game->mario.sprite;
    my_game->view[my_game->mario.y][my_game->mario.x][1] = YELLOW_TXT;


    yBkp = my_game->mario.y;
    xBkp = my_game->mario.x;

}

void showView(game_t * my_game) {
    int currentTTY = get_current_tty();
    tty[currentTTY].cursor = 0;
    check_offset('m', 4000);
    write(PANTALLA_FD, my_game->view, 4000);
    tty[currentTTY].cursor = 0;
}






void init_map_view(game_t * my_game){
    int i,j;
    for (i=0; i<25;i++) {
        for (j=0; j<80;j++) {
            my_game->view[i][j][0] = my_game->map[i][j];
            my_game->view[i][j][1] = WHITE_TXT;
        }
    }
}


void init_mario(game_t * my_game) {
    my_game->mario.x = 40;
    my_game->mario.y = 12;
    my_game->mario.sprite = 1;
    my_game->mario.alive = 1;

}


void init_hearts(game_t * my_game) {

    static pointT start_coords[HEART_N] = {
        {0,1}, {11,0}, {5,15}, {1,23}, {23,23}, 
        {7,21}, {23,1}, {23,7}, {13,11}, {7,9},
        {21,13}, {5,5}, {23,19}, {21,5}, {3,3}
    };


    int i;
    for (i = 0; i< HEART_N; i++) {
        my_game->corazones[i].x = start_coords[i].x;
        my_game->corazones[i].y = start_coords[i].y;
        my_game->corazones[i].sprite = 3;
        my_game->corazones[i].alive = 1;
    }

}


int can_move(game_t * my_game, int dx, int dy) {

    return my_game->map[(my_game->mario.y+dy)][(my_game->mario.x+dx)] == ' ';

}


