;This program first check the mode of processor(Real or Protected),
;then reads GDTR, LDTR, IDTR, TR & MSW and displays the same.

section .data
	rmodemsg db 10,'Processor is in Real Mode'
	rmsg_len:equ $-rmodemsg

	pmodemsg db 10,'Processor is in Protected Mode'
	pmsg_len:equ $-pmodemsg

	gdtmsg db 10,'GDT Contents are::'
	gmsg_len:equ $-gdtmsg

	ldtmsg db 10,'LDT Contents are::'
	lmsg_len:equ $-ldtmsg

	idtmsg db 10,'IDT Contents are::'
	imsg_len:equ $-idtmsg

	trmsg db 10,'Task Register Contents are::'
	tmsg_len: equ $-trmsg

	mswmsg db 10,'Machine Status Word::'
	mmsg_len:equ $-mswmsg

	colmsg db ':'

	nwline db 10

section .bss
	gdt resd 1
	    resw 1
	ldt resw 1
	idt resd 1
	    resw 1
	tr  resw 1

	cr0_data resd 1

	dnum_buff resb 04

%macro disp 2
	mov eax,04
	mov ebx,01
	mov ecx,%1
	mov edx,%2
	int 80h
%endmacro

section .text
	global _start
_start:	
	smsw eax		;Reading CR0

	mov [cr0_data],eax

	bt eax,1		;Checking PE bit, if 1=Protected Mode, else Real Mode
	jc prmode
	disp rmodemsg,rmsg_len
	jmp nxt1

prmode:	disp pmodemsg,pmsg_len

nxt1:	sgdt [gdt]
	sldt [ldt]
	sidt [idt]
	str [tr]
	disp gdtmsg,gmsg_len
	
	mov bx,[gdt+4]
	call disp_num

	mov bx,[gdt+2]
	call disp_num

	disp colmsg,1

	mov bx,[gdt]
	call disp_num

	disp ldtmsg,lmsg_len
	mov bx,[ldt]
	call disp_num

	disp idtmsg,imsg_len
	
	mov bx,[idt+4]
	call disp_num

	mov bx,[idt+2]
	call disp_num

	disp colmsg,1

	mov bx,[idt]
	call disp_num

	disp trmsg,tmsg_len
	
	mov bx,[tr]
	call disp_num

	disp mswmsg,mmsg_len
	
	mov bx,[cr0_data+2]
	call disp_num

	mov bx,[cr0_data]
	call disp_num

	disp nwline,1
exit:	mov eax,01
	mov ebx,00
	int 80h

disp_num:
	mov esi,dnum_buff	;point esi to buffer

	mov ecx,04		;load number of digits to display 

up1:
	rol bx,4		;rotate number left by four bits
	mov dl,bl		;move lower byte in dl
	and dl,0fh		;mask upper digit of byte in dl
	add dl,30h		;add 30h to calculate ASCII code
	cmp dl,39h		;compare with 39h
	jbe skip1		;if less than 39h skip adding 07 more 
	add dl,07h		;else add 07
skip1:
	mov [esi],dl		;store ASCII code in buffer
	inc esi			;point to next byte
	loop up1		;decrement the count of digits to display
				;if not zero jump to repeat

	disp dnum_buff,4	;display the number from buffer
	
	ret

;OUTPUT:

;[mahendra@(none) alp]$ nasm -f elf64 msmalc03.asm
;[mahendra@(none) alp]$ ld -o msmalc03 msmalc03.o
;[mahendra@(none) alp]$ ./msmalc03

;Processor is in Protected Mode
;GDT Contents are::3F604000:007F
;LDT Contents are::0000
;IDT Contents are::81BDD000:0FFF
;Task Register Contents are::0040
;Machine Status Word::8005FFFF
;[mahendra@(none) alp]$ 
