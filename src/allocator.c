#include "../include/kc.h"
#include "../include/defs.h"

/*
** esta funcion carga en las tablas de pagina los valores
** de memoria en los que se encuentran las paginas.
*/
void
allocator_init(void) {
    PAGE * index;
    PAGE page;
    int i;

    /*
    ** cargo las paginas del kernel entre 0MB y 4MB
    */
    index = (PAGE*)dirTableSO;
    page = 0x0; /* Cargo para que el index0 de la tabla apunte a 0MB */
    for(i = 0; i < 1024 ; i++, index++, page += PAGE_SIZE) {
	* index = page; /* cargo la tabla de paginas del SO con las paginas */
	* index = (* index) | 0x01; /* pongo las paginas como P = 1*/
    }

    /*
    ** cargo las paginas de los procesos entre 6MB y 10MB
    */
    index = (PAGE*)dirTableAPP; /* Cargo la direccion de la primer tabla de procesos */
    page = 0x00C00000; /* Cargo para que el primer indice de la tabla apunte a 6MB */
    for(i = 0; i < 1024; i++, index++, page += PAGE_SIZE) {
	    * index = page; /* referencio cada index de la tabla de paginas del proceso a una pagina */
    }
}

/*
** devuelve un puntero a una pagina que este libre.
** con el parametro se pide la cantidad de paginas contiguas que se reservan.
** si no las hay retorna NULL.
*/

int
hayContiguos(PAGE* dir, int cant) {
    while(cant--) {
        if(p_isLibre((int)dir))
            return 0;
    }
    return 1;
}

PAGE *
palloc(int cant) {
    PAGE ** index = (PAGE**)dirTableAPP; /* voy a recorrer la tabla de procesos */
    int i;

    for(i = 0; i < 1024 - cant; i++, index += cant) {
        if(hayContiguos(*index,cant)) {
            while(cant--) {
                * index = (PAGE*)((int)(*index) | 0x01); /* pongo P en 1 */
                * index = (PAGE*)((int)(*index) | 0x00000400); /* pongo el bit de usado en 1*/
            }
            return *index; /* retorno un puntero a una pagina */
        }
    }
    return (PAGE*)0;
}

/*
** recibe un puntero a una pagina y la libera.
** tambien recibe la cantidad de paginas que se habian pedido para hacer alloc
*/
void
pfree(PAGE * page, int cant) {
    PAGE ** index = (PAGE**)dirTableAPP; /* voy a recorrer la tabla de procesos */
    int i = 0;

    while(i < 1024) {
	if( *index == page ) { /* busco el index */
            while(cant--) {
                *index = (PAGE*)((int)(*index) & 0xFFFFFFFE); /* pongo su P en 0 */
                *index = (PAGE*)((int)(*index) & 0xFFFFFDFF); /* pongo el bit de usado en 0 */
                index++; /* paso al siguiente indice de la tabla */
            }
            return;
        }
    }
}


/*
** recibe un puntero a una pagina y le pone en 1 su bit P.
*/
void
up_p(PAGE * page) {
    PAGE ** index = (PAGE**)dirTableAPP; /* voy a recorrer la tabla de procesos */
    int i = 0;

    while(i < 1024) {
	if( *index == page ) { /* busco el index */
                *index = (PAGE*)((int)(*index) | 0x01); /* pongo su P en 1 */
            }
            return;
    }
}

/*
** recibe un puntero a una pagina y le pone en 0 su bit P.
*/
void
down_p(PAGE * page) {
    PAGE ** index = (PAGE**)dirTableAPP; /* voy a recorrer la tabla de procesos */
    int i = 0;

    while(i < 1024) {
	if( *index == page ) { /* busco el index */
                *index = (PAGE*)((int)(*index) & 0xFFFFFFFE); /* pongo su P en 0 */
            }
            return;
    }
}

extern void * eokl;
char * mem;

void * malloc(int size){
    return mem+=size;
}

void free(void * mem){
}


