int isalpha(int c){
    return (('a' <= c &&  c <= 'z' )||('A' <= c && c <= 'Z'));
}

int isdigit(int c){
    return ('0' <= c && c <= '9');
}

int isalphanum(int c){
  return isdigit(c) && isalpha(c);
}

int abs(int a){
    return (a>0)?a:-a;
}

int max(int a, int b){
    return (a>b)?a:b;
}

int min(int a, int b){
    return (a<b)?a:b;
}



void wait(int t) {
    int i,j;
    int a;
    
    for (i=0; i<t*100;i++) {
        for (j=0; j<t*100;j++) {
           a <<= 1; 
           a += a-- + 0xfb0c28c3;
           a = a >> 4;
        }
    }
}


void itoa(int num, char v[]){
	
	if(num == 0)
		return;
	else
		itoa(num/10, v+1);
		v[0]=num%10+'0';
}
