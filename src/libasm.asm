GLOBAL  _read_msw,_lidt
GLOBAL  _int_08_hand
GLOBAL  _int_80_hand
GLOBAL	_int_09_hand
GLOBAL  _int_74_hand
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti
GLOBAL  _debug
GLOBAL  _int_80_caller
GLOBAL	enable_mouse
GLOBAL	wpantalla
GLOBAL 	disable_text_cursor

EXTERN  int_08
EXTERN  SaveESP
EXTERN  GetTemporaryESP
EXTERN  LoadESP
EXTERN	leoteclado
EXTERN 	leomouse
EXTERN	next_char
EXTERN	mouse_now
EXTERN	screenShow
EXTERN	fetch
EXTERN  allocator_init



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


; Version vieja del int 08
; _int_08_hand:				; Handler de INT 8 ( Timer tick)
;         push    ds
;         push    es                      ; Se salvan los registros
;         pusha                           ; Carga de DS y ES con el valor del selector
;         mov     ax, 10h			; a utilizar.
;         mov     ds, ax
;         mov     es, ax
; 
;         call    int_08
; 
;         mov	al,20h			; Envio de EOI generico al PIC
; 	out	20h,al
; 	popa
;         pop     es
;         pop     ds
;         iret


_int_08_hand:               ; Handler de INT 8 ( Timer tick)

        
;         push    ds
;         push    es                      ; Se salvan los registros
        cli;
        pushad                           ; Carga en DS y ES el valor del selector
                                        ; a utilizar.
;         mov     ax, 10h         
;         mov     ds, ax
;         mov     es, ax
   
        mov     eax,esp

;         call    _debug
        
        push    eax
        call    SaveESP
        pop     eax
        call    GetTemporaryESP
        mov     esp, eax
        
        call    int_08
        
        call    LoadESP
        mov     esp,eax

        popad
        
;         pop     es
;         pop     ds

        push    eax
        mov     al,20h          ; Envio de EOI generico al PIC
        out     20h,al
        pop     eax
        
        sti
        iret


;Handler de la interrupcion 09 del teclado
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



;Handler de la interrupcion 74, del teclado
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
	call	mouseWait2	; espero para poder enviar datos al mouse.
	mov	al,0A8h
	out	64h,al		; activo el dispositivo auxiliar del mouse.
	
	call	mouseWait2	; espero para poder enviar datos al mouse.
	mov	al,20h
	out	64h,al 		; envio 20h al puerto 64h para que el mouse mande el byte de estado

	call	mouseWait1	; espero para poder recibir datos del mouse.
	in	al,60h		; recibo el byte de estado del mouse
	mov	ah,al
	or	ah,2		; habilito al mouse para que mande interrupciones

	call	mouseWait2 	; espero para poder enviar datos al mouse.
	mov	al,60h
	out	64h,al		; envio 60h al puerto 64h para avisar que se va a enviar el byte de estado de nuevo.

	call	mouseWait2 	; espero para poder enviar datos al mouse.
	mov	al,ah
	out     60h,al		; envio el byte de estado.
		
	call	mouseWait2 	; espero para poder enviar datos al mouse.
	mov	al,0D4h	
	out	64h,al		; envio D4h al puerto 64h para decirle al mouse que se va a enviar un comando.
	call	mouseWait2 	; espero para poder enviar datos al mouse.
	mov	al,0F6h		
	out	60h,al		; envio el comando de cargar la configuracion default del mouse.
	
	call	mouseWait1	; espero para poder recibir datos del mouse.
	in	al,60h

	call	mouseWait2	 ; espero para poder enviar datos al mouse.
	mov	al,0D4h
	out	64h,al		; envio D4h al puerto 64h para decirle al mouse que se va a enviar un comando.
	call	mouseWait2 	; espero para poder enviar datos al mouse.
	mov	al,0F4h		
	out	60h,al		; habilito el mouse
	
	call	mouseWait1	; espero para poder recibir datos del mouse.
	in	al,60h

	ret
	
	
; Espera para poder recibir datos del mouse, tiene un timeout por si no se recibe nada.	
mouseWait1:
	push	ecx
	push	eax
	mov	ecx,100000
	
ciclo1:	
	in	al,64h
	and	al,1
	cmp	al,1		; si recibe un 1 en el primer bit puedo recibir datos
	jz	mwait1_end
	dec	ecx		; si no recibe un 0 en el segundo bit, decremento el timer,
	jnz	ciclo1		; asi no queda en un ciclo infinito.

mwait1_end:
	pop	eax
	pop	ecx
	ret

; Espera para poder enviar datos al mouse, tiene un timeout por si no se recibe nada.
mouseWait2:
	push	ecx
	push	eax
	mov	ecx,100000
	
ciclo2:	
	in	al,64h		
	and	al,2		; si recibe un 0 en el segundo bit puedo enviar datos
	cmp	al,0		
	jz	mwait2_end	
	dec	ecx		; si no recibe un 0 en el segundo bit, decremento el timer,
	jnz	ciclo2		; asi no queda en un ciclo infinito.

mwait2_end:
	pop	eax
	pop	ecx
	ret


; Función que escribe en una determinada posición de pantalla un caracter. 
; El primer parámetro que recibe la función es el caracter a imprimir y
; el segundo que recibe es donde imprimir el caracter.
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
    
	mov	ebx,0B8000h			; Escribo en la pantalla
	add	ebx,ecx
	
	mov	[ds:ebx],dl			; Copio en la posicion de memoria el char a escribir
	
	pop	ecx
	pop	edx
	pop	ebx
	mov	esp,ebp
	pop	ebp
   	ret


; Función que deshabilita el cursor de texto para que no se muestre en pantalla.
; Este cursor esta seteado inicialmente y cuando se escribe el caracter de atributo
; correspondiente se puede ver a menos que sea deshabilitado ingresando a los registros
; de la placa de video.
disable_text_cursor:
	push	ebp
	mov	ebp,esp
	push	eax
	push	ecx
	push	edx

	mov	dx, 03D4h
	in	al, dx

	mov	cl, al
	mov	al, 0Ah		;ingreso al registro de índice 0Ah de la placa de video
	out	dx, al

	mov	dx, 03D5h
	in	al, dx
	or	al, 20h		;Seteo el 6 bit del registro en 1 para que desaparezca el cursor
	out	dx, al
	mov	al, cl

	mov	dx, 03D4h
	out	dx, al

	pop	edx
	pop	ecx
	pop	eax
	mov	esp,ebp
	pop	ebp

	ret


;funcion que habilita paginacion.
;ademas carga el registro CR3 y los index 0 y 1 de
;la tabla de directorios.
init_pagination:
	push ebp			; armo el stack frame
        mov ebp, esp			;
	push edx			; salvo EDX
	push ecx			; salvo ECX
        push ds                         ; salvo DS

        mov eax, 00800000h		; cargo EAX con 8MB que es donde voy a meter el directorio.
        and eax, 0FFFFF000h		; pongo los ultimos 12 bits de CR3 en 0 (ya estan igual)
        mov cr3, eax			; cargo CR3 con este valor.

        mov ax, 10h
        mov ds, ax

	mov eax, cr3			; cargo EAX con el valor de CR3.

	mov edx, 00801000h		; cargo en EDX el valor izquierdo de dirTableSO.
	or edx, 1h			; pongo el P = 1 de la tabla de paginas del SO.
        mov [ds:eax], edx		; cargo en lo que apunta CR3 (index0) 8MB+4KB (dirTableSO).

	add eax, 4h      		; sumo 4 bytes a EAX para cargar el index1.
	add edx, 1000h                  ; sumo 4KB a edx para que ahora apunte a dirTableAPP.
	or edx, 1h			; pongo el P = 1 de la tabla de paginas de los procesos.
        mov [ds:eax], edx		; cargo en lo que apunta CR3 (index1) 8MB+8KB (dirTableAPP).
	
	mov eax, cr0
        or  eax, 80000000h		; pongo el bit31 de CR0, que es PG en 1.
        mov cr0, eax
        call allocator_init             ; llamo a la funcion que carga las tablas.

        pop ds                          ; recupero DS
	pop ecx                         ; recupero ECX
	pop edx			        ; recupero EDX
	mov esp, ebp			; desarmo el stack frame
	pop ebp				;
	ret				; retorno.

; Debug para el BOCHS, detiene la ejecución; Para continuar colocar en el BOCHSDBG: set $eax=0
;

_debug:
        push    bp
        mov     bp, sp
        push	ax
vuelve:	mov     ax, 1
        cmp	ax, 0
	jne	vuelve
	pop	ax
;     mov     sp,bp
	pop     bp
        retn

