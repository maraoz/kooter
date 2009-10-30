
#ifndef _game_h_
#define _game_h_



#define HEART_N 15

typedef struct object {
    char sprite;
    int x;
    int y;
    int alive;
} object;

typedef object pjT;

typedef struct pointT {
    int x;
    int y;
} pointT;

typedef struct {
    char map[25][80];
    char view[25][80][2];
    pjT mario;
    object corazones[HEART_N];
} game_t;

int game();

void updateView(game_t * my_game);

void showView(game_t * my_game);

void init_mario(game_t * my_game);

void init_map_view(game_t * my_game);

void init_hearts(game_t * my_game);



#endif
