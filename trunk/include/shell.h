#define HIST_LEN 10

int switch_tty(int new_tty);

void print_nline();

int str_cmp(char *s, char *t);

void separaPorEspacios(char *s, char out[][LONG_STR_TKN]);

int llamaFunc(char s[][LONG_STR_TKN]);

void shell();

int atoi(char *s);

void setTimeSp(int time);

void garbage();

void check_screen_saver();

int str_cpy(char *s, char *t);

int str_len(char *s);
