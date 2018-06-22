
%macro cmn 4			;common macro for input/output
	mov rax,%1
	mov rdi,%2
	mov rsi,%3
	mov rdx,%4
	syscall
%endmacro

section .data
	num db 00h
	msg db "Factorial is : "
	msglen equ $-msg
	msg1 db "*****Program to find Factorial of a number***** ",0Ah
		db "Enter the number : ",
	msg1len equ $-msg1
	nwline db 10

section .bss
	dispbuff resb 16
	numascii resb 3
	

section .text
global _start
_start:
	
	cmn 1,1,msg1,msg1len
	cmn 0,0,numascii,3			;accept number from user
	call convert			;convert number from ascii to hex
	mov [num],bl
	
	cmn 1,1,msg,msglen
	
	xor rdx,rdx
	xor rax,rax
	mov bl,[num]

	call proc_fact 

	mov rbx,rax
	
	call display

	mov rax,60
	mov rdi,0
	syscall

	proc_fact: 
	cmp bl, 1 
	jg do_calculation 
	mov ax, 1 
	ret 

	do_calculation: 
	dec bl 
	call proc_fact 
	inc bl 
	mul bl ;ax = al * bl 
	ret 




display:			; procedure to convert hex to ascii

mov ecx,16
mov edi,dispbuff
again:rol rbx,4
	mov al,bl
and al,0fh
cmp al,09h
jbe x1
add al,07h
x1:add al,30h
mov [edi],al
inc edi
loop again
cmn 1,1,dispbuff,16
cmn 1,1,nwline,1
	ret

	

convert:			;procedure to convert ascii to hex
mov rsi,numascii
mov cl,02h
xor rax,rax
xor rbx,rbx
contc:	rol bl,04h
mov al,[rsi]
cmp al,39h
jbe skipc
sub al,07h
skipc:
sub al,30h
add bl,al
inc rsi
dec cl 
jnz contc
	
ret

	
