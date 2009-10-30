#include "../include/kc.h"
#include "../include/kasm.h"
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
palloc(int cant)
{
    static int * p = (int*)MemStart;
    p+=4096;
    up_p(p,1);
    return p;
// 	static unsigned int i;
//     static unsigned int j;
//     static unsigned int k;
//     int correct = 1;
//     
//     if(i + cant > 1023) {
//         i = 0;
//     }
// 
// 	for( ; i < (MemSize/MEM_BLOCK); )
// 	{
//         for( k = i; k < cant; k++) {
//             correct *= (!getbits8(MemMap[k/8],k%8,1))? 1 : 0;
//         }
// 		if( correct )
// 		{
//             for( j = i; j < cant; j++) {
//                 setbits8(&(MemMap[j/8]),j%8,1,1);
//             }
//             up_p((PAGE*)(MemStart+i*MEM_BLOCK), cant);
//             i = i + cant;
// 			return (void*)(MemStart+i*MEM_BLOCK);
// 		}
//         i++;
// 	}
// 
// 	return NULL;
}

void
pfree(PAGE * p, int cant)
{
	static int mempos;
	static unsigned int i;
    static unsigned int j;
    
	mempos = (int) p;

	mempos -= MemStart;
	i = mempos/MEM_BLOCK;

	if(mempos % MEM_BLOCK != 0)
	{
		/** ACA SE ROMPE TODO!!! */
		/** Se esta liberando una direccion no valida. */
	}

	if( !getbits8(MemMap[i/8], i % 8, 1) )
	{
		/** DOUBLE FREE EXCEPTION. **/
	}

	/** Todo ok, dejo libre la zona de memoria. */
    for( j = i; j < cant; j++) {
        setbits8(&(MemMap[j/8]), j % 8, 1, 0);
    }
}


void
up_p(PAGE * p, int cant)
{
	unsigned int phys = (unsigned int)p;
	unsigned int dirIndex = (phys >> 22) & 0x3FF;
	unsigned int tabIndex = (phys >> 12) & 0x3FF;
	unsigned int * table = (unsigned int*)(dirT[dirIndex] & 0xFFFFF000);

	dirT[dirIndex] = dirT[dirIndex] | 3;	//Set the directory as present
    int i;
    for(i = 0; i < cant; i++) {
        table[tabIndex + i] = (phys & 0xFFFFF000 + i * PAGE_SIZE) | 3;
    }
}

void
down_p(PAGE *p, int cant)
{
	unsigned int phys = (unsigned int)p;
	unsigned int dirIndex = (phys >> 22) & 0x3FF;
	unsigned int tabIndex = (phys >> 12) & 0x3FF;
	unsigned int * table = (unsigned int*)(dirT[dirIndex] & 0xFFFFF000);

	dirT[dirIndex] = dirT[dirIndex] | 3;	//Set the directory as present
    int i;
    for(i = 0; i < cant; i++) {
        table[tabIndex + i] = (phys & 0xFFFFF000 + i * PAGE_SIZE) & 0xFFFFFFFE;
    }
}

char * mem;

void * malloc(int size){
    return mem+=size;
}

void free(void * mem){
}


