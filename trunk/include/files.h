#ifndef _files_h_
#define _files_h_


int fs_init(void);

/**
 * Para los archivos
 */


int open(char * name);

int close(char * name);
 
int fread(char * name,char * buffer); 

int fwrite(char * name, char * input);

int get_next_file_entry();
 
boolean is_valid_fd(int index);

int get_fd(char * name);

int exists_file(char * name);

int ls(int argc, char * directory[]);

/**
 * Para los tags
 */
 
int tags(void);

int mkdir(int argc, char * directory[]);

int rmdir(int argc, char * directory[]); 

int chdir(int argc, char * directory[]);

int renamedir(int argc, char * directory[]);

/**
 * Varios
 */ 

dword get_next_available_tag(void);

dword get_numeric_tag(char * name);

int get_next_file_entry(void);

int log2(dword num);

#endif
