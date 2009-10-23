#include "../include/defs.h"
#include "../include/files.h"
#include "../include/stdio.h"
#include "../include/shell.h"

files_entry opened_files[MAX_QTY_FILES] = {{{0,0},FALSE,0}};
tag_list_t tag_list[MAX_QTY_TAGS] = {{{0},0}};
dword cwd = 0;

/**
 * Funcion que inicializa el fs
 *
 */

int
fs_init(){
    int i;
    for(i = 0; i<MAX_QTY_FILES ; i++){
        opened_files[i].used = FALSE;
        opened_files[i].references = 0;
    }
    for(i = 0; i<MAX_QTY_TAGS ; i++){
        tag_list[i].references = 0;
    }
}

/**
 * Funcion que crea un archivo en caso de no existir
 */

int
open(char * name){
    int index,i,j;
    files_t file;
    file.index = 0;
    strncpy(file.name,name,100);
    file.tags = cwd;
    file.data = (char*)palloc(1); /* numero magico, poner una valor mejor */
    index = get_next_file_entry();
    if(index == -1){
        return -1;
    }
    opened_files[index].file = file;
    opened_files[index].used = TRUE;
    opened_files[index].references++;
    for(i=file.tags,j=0;i>=0;i>>=2,j++){
        if(i%2 != 0){
           tag_list[j].references++;
        }
    }
    return index;
}

/**
 * Funcion que cierra un archivo abierto
 */
int
close(int index){
    if(!is_valid_fd(index) || opened_files[index].references == 0 || opened_files[index].used == FALSE)
        return -1;
    opened_files[index].references--;
    return index;
}


/**
 * Funcion que lee un archivo abierto
 * Hay que ver si mergeamos las dos funciones de write y read que tenemos (la de stdio y la de files)
 */
int
fread(char * name,char * buffer){
    int i;
    int index;
    index = get_fd(name);
    if(!is_valid_fd(index) || opened_files[index].used == FALSE)
        return -1;
    for(i=0;opened_files[index].file.data[i]!=0;i++){
       buffer[i] = opened_files[index].file.data[i];
    }
}

/**
 * Funcion que escribe un archivo un archivo abierto
 */

int
fwrite(char * name, char * input){
    int index,i;
    index = get_fd(name);
    if(!is_valid_fd(index) || opened_files[index].used == FALSE)
        return -1;
    for(i=0; i<MAX_FILE_SIZE-1 && input[i] !=0;i++){
        opened_files[index].file.data[i] = input[i];
    }
    opened_files[index].file.data[i] = 0;
    return i;
}

/**
 * Funcion que borra un archivo
 */
int
unlink(char * name){
    int index,i,j;
    index = get_fd(name);
    if(index == -1)
        return -1;
    opened_files[index].used = FALSE;
    for(i=opened_files[index].file.tags,j=0;i>=0;i>>=2,j++){
        if(i%2 != 0){
           tag_list[j].references--;
        }
    }
}

/**
 * Funcion que devuelve el fd de un archivo
 */

int
get_fd(char * name){
    //TODO: Implementar, hoy me quede sin tiempo
}

/*
 * Funcion que chequea que el fd sea valido
 */
boolean
is_valid_fd(int index){
    if(index < 0 || index > MAX_QTY_FILES)
        return FALSE;
    return TRUE;
}

/**
 * Funcion que cambia de directorio
 */
int
chdir(char * directory){
    dword tag;
    tag = get_numeric_tag(directory);
    if(tag == -1){
        return -1;
    }
    cwd |= tag;
}

/**
 * Funcion que lista el contenido de un directorio
 */
int
ls(char * directory){
    dword tag;
    int i;
    if(directory[0] == 0){
        tag = cwd;
    } else {
        tag = get_numeric_tag(directory)|cwd;
    }
    for(i = 0 ; i<MAX_QTY_FILES ; i++){
        if(opened_files[i].used == TRUE && opened_files[i].file.tags&tag == tag){
            putln(opened_files[i].file.name);
        }  
    }
     return 1;
}

/**
 * Funcion que crea directorios
 */
int
mkdir(char * directory){
    dword tag;
    int i;
    tag = get_next_available_tag();
    if(tag == -1){
        return -1;
    }
    i = log2(tag);
    strncpy(tag_list[i].name,directory,100);
}

/**
 * Funcion que elimina directorios
 */
int
rmdir(char * directory){
    dword tag;
    int index;
    tag = get_numeric_tag(directory);
    if(tag == -1)
        return -1;
    tag_list[log2(tag)].name[0]=0;
    return tag;
}


/**
 * Funcion que devuelve le proximo tag posible
 */ 
dword
get_next_available_tag(){
    int i;
    for(i = 0; i<MAX_QTY_TAGS && tag_list[i].name[0]!=0 ; i++);
    if(i == MAX_QTY_TAGS){
        return -1;
    }
    return 1<<i /*lo mismo que pow(2,i)*/;
}

/**
 * Funcion que devuelve el proximo indice disponile
 */
int
get_next_file_entry(){
    static int index = 0;
    if(index+1 >= MAX_QTY_FILES){
        int i;
        for(i =0 ; i<MAX_QTY_FILES; i++){
            if(opened_files[i].used = FALSE){
                return i;
            }
        }
    }
    return index++;
}

/**
 * Funcion que devuelve un tag numerico a partir de un nombre de directorio
 */

dword
get_numeric_tag(char * name){
    int i;
    for(i = 0; i<MAX_QTY_TAGS && !str_cmp(tag_list[i].name,name) ; i++);
    if(i == MAX_QTY_TAGS){
        return -1;
    }
    return 1<<i;
}

/**
 * Funcion que desde un tag te da el indice del vector de tags donde se encuentra;
 */

int
log2(dword num){
    int i;
    while(num & 0x01 == 0){
        i++;
        num=num>>1;
    }
    return i;
}