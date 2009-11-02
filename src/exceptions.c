void            _int_00_hand(){
    char * m = (char *) 0x0B8000;
    m[0]='0';m[1]='0';
    while(1);
}
void            _int_01_hand(){
    char * m = (char *) 0x0B8000;
    m[0]='1';m[1]='0';
    while(1);
}
void            _int_02_hand(){
    char * m = (char *) 0x0B8000;
    m[0]='2';m[1]='0';
    while(1);
}
void            _int_03_hand(){
    char * m = (char *) 0x0B8000;
    m[0]='3';m[1]='0';
    while(1);
}
void            _int_04_hand(){
    char * m = (char *) 0x0B8000;
    m[0]='4';m[1]='0';
    while(1);
}
void            _int_05_hand(){
    char * m = (char *) 0x0B8000;
    m[0]='5';m[1]='0';
    while(1);
}
void            _int_06_hand(){
    char * m = (char *) 0x0B8000;
    m[0]='6';m[1]='0';
    while(1);
}
void            _int_07_hand(){
    char * m = (char *) 0x0B8000;
    m[0]='7';m[1]='0';
    while(1);
}
void            _int_0A_hand(){
    char * m = (char *) 0x0B8000;
    m[0]='8';m[1]='0';
    while(1);
}
void            _int_0B_hand(){
    char * m = (char *) 0x0B8000;
    m[0]='9';m[1]='0';
    while(1);
}
void            _int_0C_hand(){
    char * m = (char *) 0x0B8000;
    m[0]='1';m[1]='0';
    m[2]='0';m[3]='0';
    while(1);
}
void            _int_0D_hand(){
    putln("Page Fault");
    while(1);
}
void            _int_0E_hand(){
    char * m = (char *) 0x0B8000;
    m[0]='1';
    m[1]='0';
    m[2]='2';
    m[3]='0';
    while(1);
}
void            _int_0F_hand(){
    char * m = (char *) 0x0B8000;
    m[0]='1';
    m[1]='0';
    m[2]='3';
    m[3]='0';
    while(1);
}
void            _int_10_hand(){
    char * m = (char *) 0x0B8000;
    m[0]='1';
    m[1]='0';
    m[2]='4';
    m[3]='0';
    while(1);
}
void            _int_11_hand(){
    char * m = (char *) 0x0B8000;
    m[0]='1';
    m[1]='0';
    m[2]='5';
    m[3]='0';
    while(1);
}
