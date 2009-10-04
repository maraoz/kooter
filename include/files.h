#ifndef _files_h_
#define _files_h_

#define MAX_QTY_FILES 1000
#define MAX_QTY_TAGS 31 /* es la cantidad de bits disponibles */

typedef struct {
    files_t file;
    boolean used = FALSE;
    int references = 0; /* referencias a este fd */
} files_entry;

typedef struct {
    char name[100];
    char * data;
    int index = 0;
    dword tags; /* los tags van a ser bits */
} files_t;

typedef struct {
    char name[100];
    int references = 0;
} tag_list_t;

int open(char * name);

int close(int index);
 
int read(char * name,char * buffer); 

int write(char * name, char * input);
 
int unlink(char * name);

boolean is_valid_fd(int index);

int chdir(char * directory);

int ls(char * directory);

int mkdir(char * directory);

int rmdir(char * directory); 

dword get_next_available_tag();

dword get_numeric_tag(char * name);

#endif