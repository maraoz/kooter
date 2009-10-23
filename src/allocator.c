#include "../include/kc.h"
#include "../include/defs.h"
#include "../include/allocator.h"
/*
** esta funcion carga en las tablas de pagina los valores
** de memoria en los que se encuentran las paginas.
*/

static char MemMap[MAP_SIZE];
static PAGE * dirT;
static PAGE * mapStart;


extern void * eokl;

inline void setbits8(unsigned char * x, char p, char n, unsigned char y)
{
	/*Borra los bits que queremos setear*/
	*x &= (~0u << (p+n)) | ~(~0u << p);
	/*Setea los bits*/
	*x |= ((y & ~(~0u << n)) << p);
}

inline unsigned int getbits8(unsigned char x, char p, char n)
{
	return (x >> p) & ~(~0u << n);
}

void
init_pagination(void) {
    unsigned long address=0; // holds the physical address of where a page is
    unsigned int i,j;
    /*Directory Table*/
    dirT = (PAGE*)((char*)FOURTH_MB_MARK - PAGE_SIZE);
    /*Mapping start*/
    mapStart = (PAGE*)FOURTH_MB_MARK;

    /*Map the kernel (first 4MB)*/
    dirT[0] = (PAGE)mapStart;
    dirT[0] = dirT[0] | 3;
    for(i=0; i<1024; i++)
    {
	    /*Attribute set to: supervisor level, read/write, present(011 in
		* binary)*/
	    mapStart[i] = address | 3;
	    address = address + 4096; // 4096 = 4kb
    }
    mapStart += PAGE_SIZE/sizeof(PAGE);

    /*Map the pages that map the rest of the memory (second 4MB)*/
    dirT[1] = (PAGE)mapStart;
    dirT[1] = dirT[1] | 3;
    for(i=0; i<1024; i++)
    {
	    /*Attribute set to: supervisor level, read/write, not present(010 in
		* binary)*/
	    mapStart[i] = address | 3;
	    address = address + 4096; // 4096 = 4kb
    }
    mapStart += PAGE_SIZE/sizeof(PAGE);

    /*We have mapped the first 8MB of the memory, now we'll use the pages in
	* the second 4MB to set them in dirT to enable easy idMapping*/
    for (i=2; i<1024; i++)
    {
	    /*Clean al the pages*/
	    for (j=0; j<1024; j++)
		    mapStart[j] = 2;

	    /*Set the page in the dir table*/
	    dirT[i] = ((PAGE)mapStart) | 3;
	    mapStart += PAGE_SIZE/sizeof(PAGE);
    }

    write_cr3(dirT); // put that page directory address into CR3
    enable_page(); // set the paging bit in CR0 to 1

}

/*
** devuelve un puntero a una pagina que este libre.
** con el parametro se pide la cantidad de paginas contiguas que se reservan.
** si no las hay retorna NULL.
*/

PAGE *
palloc(void)
{
	static unsigned int i;

	for( i = 0; i < (MemSize/MEM_BLOCK); i++ )
	{
//		printf("Mem: %X\n",MemMap[i/8]);
//		if( MemMap[i/8]&(1<<i%8) )
		if( !getbits8(MemMap[i/8],i%8,1) )
		{
//			MemMap[i/8]&= ~(1<<i%8);
			setbits8(&(MemMap[i/8]),i%8,1,1);
//			printf("MemSet: %X\n",MemMap[i/8]);
			//TODO
			return (void*)(MemStart+i*MEM_BLOCK);
//			return NULL;
		}
	}

	return NULL;
}

void
pfree(PAGE * p)
{
	static int mempos;
	static unsigned int i;

	//TODO
	mempos = (int) p;

	mempos -= MemStart;
	i = mempos/MEM_BLOCK;

	if(mempos % MEM_BLOCK != 0)
	{
		/** ACA SE ROMPE TODO!!! */
		/** Se esta liberando una direccion no valida. */
	}

//	if( MemMap[i/8]&(1<<i%8) )
	if( !getbits8(MemMap[i/8], i % 8, 1) )
	{
		/** DOUBLE FREE EXCEPTION. **/
	}

	/** Todo ok, dejo libre la zona de memoria. */
//	MemMap[i/8]|=(1<<i%8);
	setbits8(&(MemMap[i/8]), i % 8, 1, 0);
}


void
up_p(PAGE * p)
{
	unsigned int phys = (unsigned int)p;
	unsigned int dirIndex = (phys >> 22) & 0x3FF;
	unsigned int tabIndex = (phys >>12) & 0x3FF;
	unsigned int * table = (unsigned int*)(dirT[dirIndex] & 0xFFFFF000);

	dirT[dirIndex] = dirT[dirIndex] | 3;	//Set the directory as present
	table[tabIndex] = (phys & 0xFFFFF000) | 3;
	table[tabIndex+1] = ((phys & 0xFFFFF000) + PAGE_SIZE) | 3;
	table[tabIndex+2] = ((phys & 0xFFFFF000) + 2 * PAGE_SIZE) | 3;
	table[tabIndex+3] = ((phys & 0xFFFFF000) + 3 * PAGE_SIZE) | 3;
}

void
down_p(PAGE *p)
{
	unsigned int phys = (unsigned int)p;
	unsigned int dirIndex = (phys >> 22) & 0x3FF;
	unsigned int tabIndex = (phys >>12) & 0x3FF;
	unsigned int * table = (unsigned int*)(dirT[dirIndex] & 0xFFFFF000);

	dirT[dirIndex] = dirT[dirIndex] | 3;	//Set the directory as present
	table[tabIndex] = (phys & 0xFFFFF000) & 0xFFFFFFFE;
	table[tabIndex+1] = ((phys & 0xFFFFF000) + PAGE_SIZE) & 0xFFFFFFFE;
	table[tabIndex+2] = ((phys & 0xFFFFF000) + 2 * PAGE_SIZE) & 0xFFFFFFFE;
	table[tabIndex+3] = ((phys & 0xFFFFF000) + 3 * PAGE_SIZE) & 0xFFFFFFFE;
}

// ---------------------------------------------------------------------------------------

// int
// hayContiguos(PAGE** dir, int cant, int desde) {
//     int i;
// 
//     for(i = 0; i < cant; i++) {
//             if( ( (int)( *dir + desde + i ) & 0x00000100) != 0 ) {
//                 return -1;
//             }
//     }
//     return desde;
// }

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

// PAGE*
// palloc() {
//         PAGE* index = (PAGE*) dirT[1];
//         static int i = 0;
// 
// 	if( i >= 1023 ) {
// 	    return (PAGE*)0;
// 	}
// 	for(i = 0; i < 1023; ) {
// 	    if( (int)(*(index+i*sizeof(int))) & 0x00000100 == 0 ) {
// 	    * (index+i*sizeof(int)) = (PAGE)((int)(*(index+i*sizeof(int))) | 0x01);
// 	    * (index+i*sizeof(int)) = (PAGE)((int)(*(index+i*sizeof(int))) | 0x00000200);
// 	    return (PAGE*)((index+(i++)*sizeof(int)));
// 	    }
// 	    i++;
// 	}
// }


/*
** recibe un puntero a una pagina y la libera.
** tambien recibe la cantidad de paginas que se habian pedido para hacer alloc
*/
// void
// pfree(PAGE * page) {
//     PAGE ** index = (PAGE**)dirT; /* voy a recorrer la tabla de procesos */
//     int i, j;
// 
//     for(i = 0; i < 1024; i++) {
// 	if( *(index+i) == page ) { /* busco el index */
// //                 *(index+i+j) = (PAGE*)((int)(*(index+i+j)) & 0xFFFFFFFE); /* pongo su P en 0 */
// //                 *(index+i+j) = (PAGE*)((int)(*(index+i+j)) & 0xFFFFFEFF); /* pongo el bit de usado en 0 */
//             }
//             return;
//         }
// }


/*
** recibe un puntero a una pagina y le pone en 1 su bit P.
*/
// void
// up_p(PAGE * page) {
//     PAGE ** index = (PAGE**)dirTableAPP; /* voy a recorrer la tabla de procesos */
//     int i;
// 
//     for(i = 0; i < 1024; i++) {
// 	if( *(index+i) == page ) { /* busco el index */
//                 *(index+i) = (PAGE*)((int)(*(index+i)) | 0x01); /* pongo su P en 1 */
//             }
//             return;
//     }
// }

/*
** recibe un puntero a una pagina y le pone en 0 su bit P.
*/
// void
// down_p(PAGE * page) {
//     PAGE ** index = (PAGE**)dirTableAPP; /* voy a recorrer la tabla de procesos */
//     int i;
// 
//     for(i = 0; i < 1024; i++) {
// 	if( *(index+i) == page ) { /* busco el index */
//                 *(index+i) = (PAGE*)((int)(*(index+i)) & 0xFFFFFFFE); /* pongo su P en 0 */
//             }
//             return;
//     }
// }


char * mem;

void * malloc(int size){
    return mem+=size;
}

void free(void * mem){
}


