#include "../include/kc.h"
#include "../include/defs.h"
#include "../include/allocator.h"
/*
** esta funcion carga en las tablas de pagina los valores
** de memoria en los que se encuentran las paginas.
*/
extern void * eokl;

void
allocator_init(void) {
    PAGE * index;
    PAGE page;
    int i;

//     char* video = (char*)0x0B8000+2004;
//     *(video) = '0';
//     *(video+1) = '0';

    /*
    ** cargo las paginas del kernel entre 4MB y 8MB
    */
    index = (PAGE*)dirTableSO;
    page = 0x00100000; /* Cargo para que el index0 de la tabla apunte a 4MB */
    for(i = 0; i < 1023 ; i++, index++, page += PAGE_SIZE) {
	* index = page; /* cargo la tabla de paginas del SO con las paginas */
	* index = (* index) | 0x01; /* pongo las paginas como P = 1*/
        * index = (* index) | 0x00000100; /* pongo el bit de usado en 1 */
    }

    /*
    ** cargo las paginas de los procesos entre 10MB y 14MB
    */
    index = (PAGE*)dirTableAPP; /* Cargo la direccion de la primer tabla de procesos */
    page = (PAGE)(eokl + 0x00800000);/*0x00A00000*/ /* Cargo para que el primer indice de la tabla apunte a 10MB */
    for(i = 0; i < 1024; i++, index++, page += PAGE_SIZE) {
        * index = page; /* referencio cada index de la tabla de paginas del proceso a una pagina */
	* index = (* index) & 0xFFFFFFFE; /* pongo las paginas como P = 0*/
        * index = (* index) & 0xFFFFFEFF; /* pongo el bit de usado en 0 */
    }
}

/*
** devuelve un puntero a una pagina que este libre.
** con el parametro se pide la cantidad de paginas contiguas que se reservan.
** si no las hay retorna NULL.
*/

int
hayContiguos(PAGE** dir, int cant, int desde) {
    int i;

    for(i = 0; i < cant; i++) {
            if( ( (int)( *dir + desde + i ) & 0x00000100) != 0 ) {
                return -1;
            }
    }
    return desde;
}

// PAGE *
// palloc(int cant) {
//     PAGE ** index = (PAGE**)dirTableAPP; /* voy a recorrer la tabla de procesos */
//     int i, j;
//     static int number = 0;
// 
//     for(i = number; i < 1024 - cant; i++, index++) {
//         if(hayContiguos(index,cant,i)) {
//             for(j = 0; j < cant; j++, index++) {
//                 * index = (PAGE*)((int)(*index) | 0x01); /* pongo P en 1 */
//                 * index = (PAGE*)((int)(*index) | 0x00000100); /* pongo el bit de usado en 1*/
// 		number = (number + 1) % 1024;
//             }
//             return *index; /* retorno un puntero a una pagina */
//         }
//     }
//     return (PAGE*)0;
// 
//     number++;
//     return (PAGE*)(index+number*PAGE_SIZE);
// }

// char* video = (char*)0x0B8000+2004;

PAGE*
palloc(int cant) {
        PAGE** index = (PAGE**) dirTableAPP;
        int i, j;

        for(i = 0; i < 1024; i++) {
            if(hayContiguos(index, cant, i) != -1) {
                for(j = i; j < cant; j++) {
//                     *(video) = ((char*)(index+j))[1]+'0';
                    * (index+j) = (PAGE*)((int)(*(index+j)) | 0x01);
//                     *(video+6) = ((char*)(index+j))[1]+'0';
                    * (index+j) = (PAGE*)((int)(*(index+j)) | 0x00000100);
                }
                return *(index+i);
            }
//             else {
//             *(video) = '0';
//             *(video+1) = '0';
//             }
        }
        return (PAGE*)0;

//     number++;
//     return (PAGE*)(index+number*PAGE_SIZE);
}


/*
** recibe un puntero a una pagina y la libera.
** tambien recibe la cantidad de paginas que se habian pedido para hacer alloc
*/
void
pfree(PAGE * page, int cant) {
    PAGE ** index = (PAGE**)dirTableAPP; /* voy a recorrer la tabla de procesos */
    int i, j;

    for(i = 0; i < 1024; i++) {
	if( *(index+i) == page ) { /* busco el index */
            for(j = 0; j < cant; j++) {
                *(index+i+j) = (PAGE*)((int)(*(index+i+j)) & 0xFFFFFFFE); /* pongo su P en 0 */
                *(index+i+j) = (PAGE*)((int)(*(index+i+j)) & 0xFFFFFEFF); /* pongo el bit de usado en 0 */
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
    int i;

    for(i = 0; i < 1024; i++) {
	if( *(index+i) == page ) { /* busco el index */
                *(index+i) = (PAGE*)((int)(*(index+i)) | 0x01); /* pongo su P en 1 */
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
    int i;

    for(i = 0; i < 1024; i++) {
	if( *(index+i) == page ) { /* busco el index */
                *(index+i) = (PAGE*)((int)(*(index+i)) & 0xFFFFFFFE); /* pongo su P en 0 */
            }
            return;
    }
}


char * mem;

void * malloc(int size){
    return mem+=size;
}

void free(void * mem){
}


