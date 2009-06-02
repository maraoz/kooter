#include "../include/kc.h"
#include "../include/defs.h"
#include "../include/kasm.h"
#include "../include/stdio.h"
#include "../include/keyboard.h"

tcirc teclator={0,0,0,{0}};

void leoteclado (int k){
	byte c;
	if(!(teclator.qty_used == TCIRC_SIZE)){
		c = ktoa(k);
		if(c != 0x00) {
            if(teclator.next_write == TCIRC_SIZE)
                teclator.next_write = 0;
            
            put_char('a');
            teclator.tcircular[teclator.next_write] = c;
            teclator.next_write++;
            teclator.qty_used++;
            if(c>=0x20) {
                put_char(c);
                flush();
            }
	    }
	}
}

byte ktoa(int c){
	static int shift = 0;
	int const keystroke[2][83]={
        {0x1B,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39
        ,0x30,0x2D,0x3D,0x08,0x09,0x71,0x77,0x65,0x72
        ,0x74,0x79,0x75,0x69,0x6F,0x70,0x5B,0x5D,0x0A
        ,0x00/*ctrl*/,0x61,0x73,0x64,0x66,0x67,0x68,0x6A
        ,0x6B,0x6C,0x3B,0x27,0x60,0x00/*Lshifht*/,0x5C
        ,0x7A,0x78,0x63,0x76,0x62,0x6E,0x6D,0x2C,0x2E
        ,0x2F,0x00/*RShift*/,0x2A,0x00/*alt*/,0x20
        ,0x00/*caps*/,0x3B00,0x3C00,0x3D00,0x3E00
        ,0x3F00,0x4000,0x4100,0x4200,0x4300,0x4400
        ,0x00/*num*/,0x00/*scrl*/,0x4700,0x4800,0x4900
        ,0x2D,0x4B00,0x4C00,0x4D00,0x2B,0x4F00,0x5000
        ,0x5100,0x5200,0x5300},
        
        {0x1B,0x21,0x40,0x23,0x24,0x25,0x5E,0x26,0x2A,0x28
        ,0x29,0x5F,0x2B,0x08,0x0F00,0x51,0x57,0x45,0x52
        ,0x54,0x59,0x55,0x49,0x4F,0x50,0x7B,0x7D,0x0D
        ,0x00/*ctrl*/,0x41,0x53,0x44,0x46,0x47,0x48,0x4A
        ,0x4B,0x4C,0x3A,0x22,0x7E,0x00/*Lshifht*/,0x7C
        ,0x5A,0x58,0x43,0x56,0x42,0x4E,0x4D,0x3C,0x3E
        ,0x3F,0x00/*RShift*/,0x00/*INT 5 PRT SCREEN*/,0x00/*alt*/,0x20
        ,0x00/*caps*/,0x5400,0x5500,0x5600,0x5700
        ,0x5800,0x5900,0x5A00,0x5B00,0x5C00,0x5D00
        ,0x00/*num*/,0x00/*scrl*/,0x37,0x38,0x39
        ,0x2D,0x34,0x35,0x36,0x2B,0x31,0x32
        ,0x33,0x30,0x2E}};
      
    if( c == 0x2A || c == 0x36)
	shift = 1;
    if( c == 0x2A+0x80 || c == 0x36+0x80)
	shift = 0;
   
    if(c<0x81)
      return (byte)0x00;
    return (byte)keystroke[shift][c-1-0x80];
}

byte next_char (){
	byte a;
	if(teclator.qty_used != 0) {
		a=teclator.tcircular[teclator.next_read];
		teclator.next_read++;
		teclator.qty_used--;
	}
	else
		a = 0;
	return a;
}
