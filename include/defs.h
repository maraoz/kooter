/***************************************************
  Defs.h
	
****************************************************/

#include "../include/queue.h"

#ifndef _defs_
#define _defs_

#define TRUE 1
#define FALSE 0
#define NULL (void*)0

#define byte unsigned char
#define word short int
#define dword int
#define boolean byte


/* Flags para derechos de acceso de los segmentos */
#define ACS_PRESENT     0x80            /* segmento presente en memoria */
#define ACS_CSEG        0x18            /* segmento de codigo */
#define ACS_DSEG        0x10            /* segmento de datos */
#define ACS_READ        0x02            /* segmento de lectura */
#define ACS_WRITE       0x02            /* segmento de escritura */
#define ACS_IDT         ACS_DSEG
#define ACS_INT_386 	0x0E		/* Interrupt GATE 32 bits */
#define ACS_EXC_386     0x0F        /* Interrupt GATE 32 bits */
#define ACS_INT         ( ACS_PRESENT | ACS_INT_386 )

#define ACS_EXCEPTION        (ACS_PRESENT | ACS_EXC_386 )
#define ACS_CODE        (ACS_PRESENT | ACS_CSEG | ACS_READ)
#define ACS_DATA        (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define ACS_STACK       (ACS_PRESENT | ACS_DSEG | ACS_WRITE)


/*   File Descriptors   */
#define PANTALLA_FD 0
#define MOUSE_FD 1
#define TECLADO_FD 2
#define TCIRC_SIZE	80*25

/* modos de la int_80 */
#define WRITE 0
#define READ 1
// typedef enum eINT_80 {WRITE=0, READ} tINT_80;

#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/*  defines para la longitud de los strings de ingreso de datos */

#define LONG_STR_TKN 100
#define LONG_STR_CMD 320

/* define para los return de las funciones en el shell */

#define AUX 0
#define ECHO_CD 1
#define CLEAR_CD 2
#define SETTIME_CD 3
#define ACTSP_CD 4
#define DSPIMG_CD 5
#define CNF_CD 6
#define NO_CD 7
#define GBG_CD 8
#define MARIO_CD 9
#define MKDIR_CD 10
#define RM_CD 11
#define PRINT_CD 12
#define PUT_CD 13
#define CHDIR_CD 14
#define CHNAMEDIR_CD 15
#define LS_CD 16
#define TAGLIST_CD 17
#define RMDIR_CD 18
#define TOP_CD 19
#define INFINITE_CD 20
#define CHNAME_CD 21
#define OPEN_CD 22
#define CODE_CD 23
#define ADDTAG_CD 24
#define RMTAG_CD 25
#define TAGSLONG_CD 26
#define FILETAGS_CD 27
#define WHEREAMI_CD 28
#define LOCATE_CD 29
#define KILL_CD 30
#define LSALL_CD 31
#define CRASH_CD 32

/* defines para las funciones de allocator */
#define FOURTH_MB_MARK 0x00400000

#define PAGE_SIZE 0x00001000
        /* 4KB */

#define MemStart 0x0

#define MemSize 0x00800000

#define MEM_BLOCK 16384

#define MAP_SIZE 32768

typedef int PAGE;

/* Descriptor de segmento */
typedef struct {
  word limit,
       base_l;
  byte base_m,
       access,
       attribs,
       base_h;
} DESCR_SEG;


/* Descriptor de interrupcion */
typedef struct {
  word      offset_l,
            selector;
  byte      cero,
            access;
  word	    offset_h;
} DESCR_INT;

/* IDTR  */
typedef struct {
  word  limit;
  dword base;
} IDTR;


typedef int size_t;
typedef short int ssize_t;


typedef struct {
	int	next_write;
	int	next_read;
	int	qty_used;
	byte 	tcircular[TCIRC_SIZE];

} tcirc;

typedef int pid_t;

typedef struct {
	int	x;
	int	y;
} point;

typedef struct {
	byte	izq;
	byte	der;
	point	pos;
} mouseSt;

int tTicks;

typedef struct {
    int pid;
    int gid;
    int priority;
    int f_pointer;
    int background;
    boolean isAlive;
    boolean isBlocked;
    char name[20];
} process_t;

typedef struct {
    process_t process;
    dword ESP;
    dword SS;
    PAGE * page;
    int tty;
    int page_qty;
    pid_t dad_pid;
} context_t;

typedef struct {
    dword EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX, EIP, CS, EFLAGS;
    void * retaddr;
    int argc;
    char ** argv;
} STACK_FRAME;

typedef struct
{
    char view[4160];
    int cursor;
    queue_t * kb_buffer;
} TTY;




/*files */

#define MAX_QTY_FILES 1000
#define MAX_QTY_TAGS 32 /* es la cantidad de bits disponibles */
#define MAX_FILE_SIZE PAGE_SIZE


typedef struct {
    char name[50];
    char * data;
    int index;
    dword tags; /* los tags van a ser bits */
} files_t;

typedef struct {
    files_t file;
    boolean used;
    int references; /* referencias a este fd */
} files_entry;

typedef struct {
    char name[50];
    int references;
} tag_list_t;



/* BLOCK CHANNELS */
#define CHANNEL_AMMOUNT 3

#define CNL_ANY -1

#define CNL_FOREVER 0
#define CNL_KEYBOARD 1
#define CNL_CHILDREN 2



#endif

