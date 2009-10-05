

extern void * eokl;
char * mem = (char*)eokl + 4096;

void * malloc(int size){
    return mem+=size;
}

void free(void * mem){
}
