GLOBAL  _read_msw,_lidt
GLOBAL  _int_08_hand
GLOBAL  _int_80_hand
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti
GLOBAL  _debug
GLOBAL  _int_80_caller

EXTERN  int_08



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
	call	sys_write
	cmp	ah,1
	call	sys_read

        pop     ds
	pop	edx
	pop	ecx
	pop	ebx
	mov	esp,ebp
	pop 	ebp
	iret
	
sys_write:
	cmp	bh,0
	jz	pantalla		; si el file descriptor es 0 (pantalla) voy a escribir a a B8000h
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
		

pantalla:

	mov 	dh,  07h			; cargo el formato (letras blancas) en que se escribe cada byte en pantalla

	mov	ax,10h			
	mov	ds,ax
	mov	ebx,0B8000h
	add	ebx,ecx
	mov	[ds:ebx],dx			; Copio en la posicion de memoria el char a escribir
        ret

sys_read:
	cmp	bh,1
	jz	mouse		; si el file descriptor es 1 -> leo del mouse
	cmp	bh,2		
	jz	teclado		; si el file descriptor es 2 -> leo del teclado
	cmp	bh,3
	jz	rmemoria		; si el file descriptor es 3 -> leo de la memoria
	ret
	
mouse:
	in	eax,60h			; leo del puerto 60h
	ret

teclado:
	in	eax,60h			; leo del puerto 60h
	ret

rmemoria:
	mov	ax,10h			
	mov	ds,ax
	mov	eax,[ds:ecx]			; Copio de la posicion de memoria que hay en ecx a eax
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
