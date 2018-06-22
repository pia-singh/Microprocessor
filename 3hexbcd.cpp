section .data
	
	msg db 10,'*********MENU************'
	msg_len equ $-msg

	m1 db 10,'1.HEX to BCD:'
	m1_len equ $-m1

	m2 db 10,'2.BCD to HEX:'
	m2_len equ $-m2

	m3 db 10,'3.EXIT'
	m3_len equ $-m3

	m4 db 10,'Enter your choice:'
	m4_len equ $-m4

	hexmsg db 10,'Enter 4 digit Hex number:'
	hexmsg_len equ $-hexmsg

	bcd_conv db 10,'Equivalent BCD conv. is:'
	bcd_conv_len equ $-bcd_conv
	
	bcdmsg db 10,'Enter 5 digit BCD number:'
	bcdmsg_len equ $-bcdmsg

	hexconv db 10,'Equivalent HEX conv. is:'
	hexconv_len equ $-hexconv
	
	
	
section .bss
	
	choice resb 02
	numascii resb 06	
	dispbuff resb 08
        dispbuff1 resb 08
	

%macro dispmsg 2
mov rax,01
mov rdi,01
mov rsi,%1
mov rdx,%2
syscall
%endmacro

%macro accept 2
mov rax,00
mov rdi,00
mov rsi,%1
mov rdx,%2
syscall
%endmacro


section .text
global _start
_start:

menu:	dispmsg msg,msg_len
	dispmsg m1,m1_len
	dispmsg m2,m2_len
	dispmsg m3,m3_len
	dispmsg m4,m4_len

	accept choice,02
	cmp byte [choice],'1'
	je H2B

	cmp byte [choice],'2'
	je B2H
cmp byte [choice],'3'
je exit

exit:
mov rax,60
mov rbx,00
syscall

	


H2B: call HEX2BCD_proc
	jmp menu 

B2H: call BCD2HEX_proc
	jmp menu


		
HEX2BCD_proc:

	dispmsg hexmsg,hexmsg_len
	
	accept numascii,06
	call read
	mov ax,bx
	mov bx,10
	mov rcx,0
back:	mov dx,0
	div bx
	push rdx
	inc rcx
	cmp ax,0
	jne back
	mov rdi,dispbuff1
	
	x1: pop rdx
 	add dl,30h
	mov [rdi],dl
	inc rdi
	loop x1
dispmsg bcd_conv,bcd_conv_len
	dispmsg dispbuff1,5 
		jmp menu
ret


BCD2HEX_proc:
	dispmsg bcdmsg,bcdmsg_len 
	accept numascii,6
         
dispmsg hexconv,hexconv_len 
	
	mov rsi,numascii
	mov rcx,05
	mov rax,0
	mov ebx,0ah
    xy:	mov rdx,0
	mul ebx
	mov dl,[rsi]
	sub dl,30h
	add rax,rdx
	inc rsi
	loop xy
	mov ebx,eax
	call display
	jmp menu
	ret



read: mov ecx,4
       mov ebx,0
       mov esi,numascii

back1:  rol ebx,4
       mov al,[esi]
       cmp al,39h
       jbe y1
       cmp al,'a'
       jb y2
       sub al,57h
       jmp y3

y1: sub al,30h
     jmp y3
y2: sub al,37h

y3: add bl,al
    inc esi
    loop back1
    ret	


display:
mov ecx,8
         mov edi,dispbuff

again: rol ebx,4
       mov al,bl
       and al,0Fh
       cmp al,9
       jbe l1
       cmp al,'a'
       jb l2
       add al,57h
       jmp l3
l1:    add al,30h
       jmp l3
l2:    add al,37h
l3:    mov [edi],al
       inc edi
       loop again
       dispmsg dispbuff,8
           ret

;Output :
;student@student-OptiPlex-3020:~$ nasm -f elf64 hexbcd.asm
;student@student-OptiPlex-3020:~$ ld -o hexbcd hexbcd.o
;student@student-OptiPlex-3020:~$ ./hexbcd

;*********MENU************
;1.HEX to BCD:
;2.BCD to HEX:
;3.EXIT
;Enter your choice:3
;student@student-OptiPlex-3020:~$ ./hexbcd

;*********MENU************
;1.HEX to BCD:
;2.BCD to HEX:
;3.EXIT
;Enter your choice:1

;Enter 4 digit Hex number:000F

;Equivalent BCD conv. is:15
;*********MENU************
;1.HEX to BCD:
;2.BCD to HEX:
;3.EXIT
;Enter your choice:2

;Enter 5 digit BCD number:00016

;Equivalent HEX conv. is:00000010
;*********MENU************
;1.HEX to BCD:
;2.BCD to HEX:
;3.EXIT
;Enter your choice:3
;student@student-OptiPlex-3020:~$ 

