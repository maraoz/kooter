GLOBAL  _read_msw,_lidt
GLOBAL  _int_08_hand
GLOBAL  _int_80_hand
GLOBAL	_int_09_hand
GLOBAL  _int_74_hand
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti
GLOBAL  _debug
GLOBAL  _int_80_caller
GLOBAL	enable_mouse
GLOBAL	mouse_reset
GLOBAL	wpantalla
GLOBAL 	disable_text_cursor


EXTERN  int_08
EXTERN	leoteclado
EXTERN 	leomouse
EXTERN	next_char
EXTERN	mouse_now
EXTERN	screenShow
EXTERN	fetch



SECTION .text


_Cli:
	cli			; limpia flag de interrupciones
	ret

_Sti:

	sti			; habilita interrupciones por flag
	ret

_mascaraPIC1:			; Escribe mascara del PIC 1
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	21h,al
        pop     ebp
        retn

_mascaraPIC2:			; Escribe mascara del PIC 2
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	0A1h,al
        pop     ebp
        retn

_read_msw:
        smsw    ax		; Obtiene la Machine Status Word
        retn


_lidt:				; Carga el IDTR
        push    ebp
        mov     ebp, esp
        push    ebx
        mov     ebx, [ss: ebp + 6] ; ds:bx = puntero a IDTR 
	rol	ebx,16		    	
	lidt    [ds: ebx]          ; carga IDTR
        pop     ebx
        pop     ebp
        retn



_int_08_hand:				; Handler de INT 8 ( Timer tick)
        push    ds
        push    es                      ; Se salvan los registros
        pusha                           ; Carga de DS y ES con el valor del selector
        mov     ax, 10h			; a utilizar.
        mov     ds, ax
        mov     es, ax

        call    int_08

        mov	al,20h			; Envio de EOI generico al PIC
	out	20h,al
	popa
        pop     es
        pop     ds
        iret


_int_09_hand:
	push	ebp
	mov	ebp,esp
	pusha

	mov	eax,00h
	in	al,60h			; leo del puerto 60h
	mov	ah,00h
	
	push	eax
	call	leoteclado

	add	esp,4
	
	mov	al,20h			; Envio de EOI generico al PIC
	out	20h,al

	popa
	mov	esp,ebp
	pop	ebp
	iret



_int_74_hand:

	push	ebp
	mov	ebp,esp
	pusha
      
	mov	eax,0
	in	al,60h			; leo del puerto 60h
	mov	ah,00h
	
	push	eax
      
	call	leomouse

	add	esp,4
	
	mov	al,20h			
	out	0A0h,al			; Envio de EOI generico al PIC2
	
	out	20h,al			; Envio de EOI generico al PIC1
      
	popa
	mov	esp,ebp
	pop	ebp
	iret



; Llama a la interrupcion 80h
; Recibe en en el primer parametro de stack si es write o read. 
; 0 si se quiere llamar a sys_read y 1 si se quiere llamar a sys_write. Es de tipo byte (8 bits).
; Recibe como segundo parametro en el stack el file descriptor. Es de tipo byte (8 bits).
; Recibe como tercer parametro de stack el offset. Es de tipo dword (32 bits).
; Recibe como cuarto parametro de stack el dato. Es de tipo byte (8 bits).
	
_int_80_caller:
	push	ebp
	mov	ebp,esp
	push	ebx
	push	ecx
	push	edx
	
	mov	ah,[ebp+8]
	mov	bh,[ebp+12]
	mov	ecx,[ebp+16]
	mov	dl,[ebp+20]
	
	int	80h

	pop	edx
	pop	ecx
	pop	ebx
	mov	esp,ebp
	pop	ebp
	ret


; Manejador de rutinas de atencion para la interrupcion 80h
; Se recibe en ah 0 si se quiere llamar a sys_read y 1 si se quiere llamar a sys_write
; Se recibe en bx el file descritpor
; Se carga en ds el segment y en bx el offset
	
_int_80_hand:				; Handler de INT 80h (sys_read  y sys_write)
	push	ebp
	mov	ebp,esp
	push	ebx
	push	ecx
	push	edx
	push    ds

	cmp	ah,0
	jnz	nswrite
	call	sys_write
	jmp	int80end
nswrite:
	cmp	ah,1
	jnz	int80end
	call	sys_read

int80end:
        pop     ds
	pop	edx
	pop	ecx
	pop	ebx
	mov	esp,ebp
	pop	ebp
	iret
	
sys_write:
	cmp	bh,0
	jnz	no_bvideo		; si el file descriptor es 0 (pantalla) voy a escribir a a B8000h
	push	ecx
	push	edx
	call	screenShow
	add	esp,8
	ret
	
no_bvideo:
	cmp	bh,3
	jz	wmemoria			; si el file descriptor es 3 (memoria) voy a escribir a a 0CAFEh
	ret
	
wmemoria:
	mov	ax,10h			
	mov	ds,ax
	mov	ebx,0CAFEh
	add	ebx,ecx
	mov	[ds:ebx],dl			; Copio en la posicion de memoria el char a escribir
	ret


sys_read: 
	cmp	bh,0
	jz	rpantalla
	cmp	bh,1
	jz	mouse		; si el file descriptor es 1 -> leo del mouse
	cmp	bh,2				
	jz	teclado		; si el file descriptor es 2 -> leo del teclado		
	cmp	bh,3
	jz	rmemoria		; si el file descriptor es 3 -> leo de la memoria
	ret

rpantalla:
	call	fetch
	ret

mouse:
	call	mouse_now			; leo del puerto 60h
	ret

teclado:
	call	next_char
	ret

rmemoria:
	mov	ax,10h			
	mov	ds,ax
	mov	eax,[ds:ecx]			; Copio de la posicion de memoria que hay en ecx a eax
	ret


; Habilita al mouse  para empezar a recibir sus interrupciones
enable_mouse:
	call	mouseWait2
	mov	al,0A8h
	out	64h,al
	
	call	mouseWait2
	mov	al,20h
	out	64h,al

	call	mouseWait1
	in	al,60h
	mov	ah,al
	or	ah,2

	call	mouseWait2
	mov	al,60h
	out	64h,al

	call	mouseWait2
	mov	al,ah
	out     60h,al
		
	call	mouseWait2
	mov	al,0D4h
	out	64h,al
	call	mouseWait2
	mov	al,0F6h
	out	60h,al
	
	call	mouseWait1
	in	al,60h

	call	mouseWait2	
	mov	al,0D4h
	out	64h,al
	call	mouseWait2
	mov	al,0F4h
	out	60h,al
	
	call	mouseWait1	
	in	al,60h

	ret
	
mouse_reset:
	call	mouseWait2	
	mov	al,0D4h
	out	64h,al
	call	mouseWait2
	mov	al,0FFh
	out	60h,al

	call	mouseWait1
	in	al,60h

	cmp	al,0AAh
	jz	resetok
	jmp	mouse_reset

resetok:
	call	enable_mouse
	
	ret
	
	
	
mouseWait1:
	push	ecx
	push	eax
	mov	ecx,100000
	
ciclo1:	
	in	al,64h
	and	al,1
	cmp	al,1
	jz	mwait1_end
	dec	ecx
	jnz	ciclo1

mwait1_end:
	pop	eax
	pop	ecx
	ret

mouseWait2:
	push	ecx
	push	eax
	mov	ecx,100000
	
ciclo2:	
	in	al,64h
	and	al,2
	cmp	al,0
	jz	mwait2_end
	dec	ecx
	jnz	ciclo2

mwait2_end:
	pop	eax
	pop	ecx
	ret


wpantalla:

	push	ebp
	mov	ebp,esp
	push	ebx
	push	edx
	push	ecx
	
	mov	dl,[ebp+8]
	mov	ecx,[ebp+12]
	
	mov	ax,10h			
	mov	ds,ax
    
	mov	ebx,0B8000h
	add	ebx,ecx
	
	mov	[ds:ebx],dl			; Copio en la posicion de memoria el char a escribir
	
	pop	ecx
	pop	edx
	pop	ebx
	mov	esp,ebp
	pop	ebp
   	ret



; disable_text_cursor:
; 	push	ebp
; 	mov	ebp,esp
; 	push	eax
; 	push	edx
; 	
; 	in	al,3DAh
; 	in	al,3C0h
; 
; 	mov	dl,al
; 
; 	mov	al,0Ah
; 	out	3C0h,al
; 
; 	in	al,3Ch
; 	mov	dh,al
; 
; 	mov	al,20h
; 	out	3C0h,al
; 
; 	pop	edx	
; 	pop	eax
; 	mov	esp,ebp
; 	pop	ebp
; 
;    	ret


disable_text_cursor:
	push	ebp
	mov	ebp,esp
	push	eax
	push	ecx

	in	al, 3D4h			;ADDRESS

	mov	cl, al
	mov	al, 0Ah
	out	3D4h, al			;ADDRESS
	in	al, 3B5h			;DATA
	or	al, 20h
	out	3B5h, al			;DATA
	mov	al, cl
	out	3D4h, al			;ADDRESS

	pop	ecx
	pop	eax
	mov	esp,ebp
	pop	ebp

	ret


; Debug para el BOCHS, detiene la ejecució; Para continuar colocar en el BOCHSDBG: set $eax=0
;

_debug:
        push    bp
        mov     bp, sp
        push	ax
vuelve:	mov     ax, 1
        cmp	ax, 0
	jne	vuelve
	pop	ax
	pop     bp
        retn

