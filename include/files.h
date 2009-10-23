#ifndef _files_h_
#define _files_h_

int fs_init(void);

int open(char * name);

int close(int index);
 
int fread(char * name,char * buffer); 

int fwrite(char * name, char * input);
 
int unlink(char * name);

boolean is_valid_fd(int index);

int chdir(char * directory);

int ls(char * directory);

int mkdir(char * directory);

int rmdir(char * directory); 

dword get_next_available_tag();

dword get_numeric_tag(char * name);

int get_next_file_entry();

#endif
