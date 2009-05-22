#include "../include/defs.h"

/********************************** 
*
*  Kernel
*
***********************************/

#ifndef _kernel_
#define _kernel_

#define OS_PID	0

int (*player)(void);

typedef int size_t;
typedef short int ssize_t;
typedef enum eINT_80 {WRITE=0, READ} tINT_80;
typedef enum eUSER {U_KERNEL=0, U_NORMAL} tUSERS;

/* write
*
* Recibe como parametros:
* - File Descriptor
* - Buffer del source
* - Cantidad
*
**/
size_t write(int fd, const void* buffer, size_t count);

/* read
*
* Recibe como parametros:
* - File Descriptor
* - Buffer a donde escribir
* - Cantidad
*
**/
size_t read(int fd, void* buffer, size_t count);

#endif
