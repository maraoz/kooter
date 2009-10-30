#ifndef _shell_proc_h
#define _shell_proc_h

int echo(int argc, char* argv[]);

int setTimeSp(int argc, char* argv[]);

int k_clear_screen(int argc, char* argv[]);

int activaSp(int argc, char* argv[]);

int dispImg(int argc, char* argv[]);

int garbage(int argc, char* argv[]);

int mario(int argc, char* argv[]);

int infinite(int argc, char* argv[]);
int print_a(int argc, char* argv[]);
int print_b(int argc, char* argv[]);

int check_screen_saver(int argc, char* argv[]);

int cat(int argc, char *argv[]);

int rm(int argc, char *argv[]);

int rmdir(int argc, char *argv[]);

int print(int argc, char *argv[]);

int put(int argc, char *argv[]);

int chnamedir(int argc, char *argv[]);

int chname(int argc, char *argv[]);

int open(int argc, char *argv[]);

int chdir(int argc, char *argv[]);

int ls(int argc, char *argv[]);

int mkdir(int argc, char *argv[]);

int taglist(int argc, char *argv[]);

int addtag(int argc, char *argv[]);

int rmtag(int argc, char *argv[]);

int tagslong(int argc, char *argv[]);

int filetags(int argc, char *argv[]);

int whereami(int argc, char *argv[]);

int locate(int argc, char *argv[]);

int wrapkill(int argc, char *argv[]);

#endif
