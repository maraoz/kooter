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



_int_80_caller:
	push	ebp
	mov	ebp,esp
	mov	ah,[ebp+8]
	mov	bh,[ebp+12]
	mov	ecx,[ebp+16]
	int	80h
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
	push	eax			; Se salvan los registros
        push    ds
        pusha
	cmp	ah,0
	jz	sys_read
	cmp	ah,1
	jnz	int_80_end
	
sys_write:

	cmp	bx,0
	jnz	int_80_end
	mov	ax,10h
	mov	ds,ax
	mov	bx,0B800h
        mov	[ds:bx],ecx			; Copio en la posicion de memoria el char a escribir
        jmp	int_80_end

sys_read:
	
      
int_80_end:
	popa
        pop     ds
	pop	eax
	mov	esp,ebp
	pop ebp
	iret


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
