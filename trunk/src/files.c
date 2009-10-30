#include "../include/defs.h"
#include "../include/files.h"
#include "../include/stdio.h"
#include "../include/shell.h"
#include "../include/kasm.h"
#include "../include/allocator.h"

files_entry opened_files[MAX_QTY_FILES];
tag_list_t tag_list[MAX_QTY_TAGS];
dword cwd[8] = {0};
extern pid_t current_process;

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
openf(char * name){
    _Cli();
    int currentTTY = get_current_tty();
    int index,i,j;
    if(exists_file(name) != -1){
        putln("El nombre de archivo ya existe");
        return -1;
    }
    files_t file;
    file.index = 0;
    strncpy(file.name,name,20);
    file.tags = cwd[currentTTY];
    file.data = (char*)palloc(1); /* numero magico, poner una valor mejor */
    index = get_next_file_entry();
    if(index == -1){
        return -1;
    }
    opened_files[index].file = file;
    opened_files[index].used = TRUE;
    opened_files[index].references++;
    for(i=file.tags,j=0;i>0;i>>=2,j++){
        if(i%2 != 0){
           tag_list[j].references++;
        }
    }
    _Sti();
    return index;
}

/**
 * Funcion que renombra un archivo
 */

int
rename(char * name, char * newname){
    _Cli();
    int index,i,j;
    int currentTTY = get_current_tty();
    if((index = exists_file(name)) == -1){
        putln("El nombre de archivo no existe");
        return -1;
    }
    if(exists_file(newname) != -1){
        putln("El nombre de archivo ya existe");
        return -1;
    }
    str_ncpy(opened_files[index].file.name,newname,20);
    _Sti();
    return index;
}

/**
 * Funcion que verifica que no exista el nombre de archivo
 */
int
exists_file(char * name){
    int i;
    for(i = 0 ; i<MAX_QTY_FILES ; i++){
	if(opened_files[i].used){
	    if(str_cmp(name,opened_files[i].file.name)){
		return i;
	    }
	}
    }
    return -1;
}

/**
 * Funcion que cierra un archivo abierto y lo borra
 */
int
closef(char * name){
    int index;
    if((index = exists_file(name)) == -1){
	putln("El nombre de archivo no existe");
	return -1;
    }
    if(!is_valid_fd(index) || opened_files[index].references == 0 || opened_files[index].used == FALSE){
	putln("No existe el archivo");
        return -1;
    }
    opened_files[index].references--;
    opened_files[index].used = FALSE;
    pfree(opened_files[index].data);
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
    index = exists_file(name);
    if(index == -1){
	putln("Archivo inexistente");
	return -1;
    }
    if(!is_valid_fd(index) || opened_files[index].used == FALSE){
	putln("El archivo no existe");
        return -1;
    }
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
    index = exists_file(name);
    if(index == -1){
	putln("Archivo inexistente");
	return -1;
    }
    if(!is_valid_fd(index) || opened_files[index].used == FALSE){
	putln("El archivo no existe");
        return -1;
    }
    for(i=0; i<MAX_FILE_SIZE && input[i]!=0;i++){
        opened_files[index].file.data[i] = input[i];
    }
    opened_files[index].file.data[i] = 0;
    return i;
}


/**
 * Funcion que devuelve el indice de un nombre de archivo
 */
/**
 * Funcion que borra un archivo
 */
// int
// unlink(char * name){
//     int index,i,j;
//     index = get_fd(name);
//     if(index == -1)
//         return -1;
//     opened_files[index].used = FALSE;
//     for(i=opened_files[index].file.tags,j=0;i>=0;i>>=2,j++){
//         if(i%2 != 0){
//            tag_list[j].references--;
//         }
//     }
// }


/**
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
chdird(char * param){
    dword tag;
    tag = get_numeric_tag(param);
    if(tag == -1){
	putln("Tag invalido");
        return -1;
    }
    cwd[currentTTY] |= tag;
}

/**
 * Funcion que lista el contenido de un directorio
 */
int
lsdir(char * param){
    _Cli();
    dword tag;
    int i;
    if(param[0] == 0){
        tag = cwd[currentTTY];
    } else {
        tag = get_numeric_tag(param)|cwd[currentTTY];
    }
    for(i = 0 ; i<MAX_QTY_FILES ; i++){
        if(opened_files[i].used == TRUE && opened_files[i].file.tags&tag == tag){
            putln(opened_files[i].file.name);
        }  
    }
    _Sti();
    return 1;
}

/**
 * Funcion que crea directorios
 */
int
mkdird(char * param){
    _Cli();
    dword tag;
    int i;
    for(i=0;i<MAX_QTY_TAGS;i++){
        if(str_cmp(param,tag_list[i].name)){
            putln("No se puede crear dos directorios con el mismo nombre");
            return -1;
        }
    }
    putln(param);
    tag = get_next_available_tag();
    if(tag == -1){
        putln("No hay mas tags disponibles");
        return -1;
    }
    i = log2(tag);
    str_ncpy(tag_list[i].name,param,20);
    _Sti();
}

/**
 * Funcion que cambia el nombre de un directorio
 */
 
int
renamedir(char * param1, char * param2){
    _Cli();
    dword tag;
    int i;
    tag = get_numeric_tag(param1);
    if (tag == -1){
	putln("No existe el tag");
	return -1;
    }
    str_ncpy(tag_list[log2(tag)].name,param2,20);
}

/**
 * Funcion que elimina directorios
 */
int
rmdire(char * param){
    _Cli();
    dword tag;
    int index;
    tag = get_numeric_tag(param);
    if(tag == -1){
	putln("No existe el tag");
        return -1;
    }
    tag_list[log2(tag)].name[0]=0;
    return tag;
    _Sti();
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
    return /*1<<*/i /*lo mismo que pow(2,i)*/;
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
 * Funcion que devuelve los tags
 */

int
tags(){
    _Cli();
    int i;
    for(i=0;i<MAX_QTY_TAGS;i++){
        if(tag_list[i].name[0]!=0)
            putln(tag_list[i].name);
    }
    return 1;
    _Sti();
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
