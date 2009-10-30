#ifndef _files_h_
#define _files_h_


int fs_init(void);

/**
 * Para los archivos
 */


int openf(char * name);

int closef(char * name);
 
int fread(char * name,char * buffer); 

int fwrite(char * name, char * input);

int get_next_file_entry();
 
boolean is_valid_fd(int index);

int get_fd(char * name);

int exists_file(char * name, dword tag);

int lsdir(char * param);

/**
 * Para los tags
 */
 
int tags(void);

int mkdird(char * param);

int rmdire(char * param); 

int chdird(char * param);

int renamedir(char * param1, char * param2);

/**
 * Varios
 */ 

dword get_next_available_tag(void);

dword get_numeric_tag(char * name);

int get_next_file_entry(void);

int log2(dword num);

#endif
