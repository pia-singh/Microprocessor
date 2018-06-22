;---------------------------------------------------------------------
section .data
	nline		db	10,10
	nline_len:	equ	$-nline

	smsg		db	10,"No. of spaces are	: "
	smsg_len:	equ	$-smsg
	
	nmsg		db	10,"No. of lines are	: "
	nmsg_len:	equ	$-nmsg

	cmsg		db	10,"No. of character occurances are	: "
	cmsg_len:	equ	$-cmsg

;---------------------------------------------------------------------
section .bss

	scount	resq	1
	ncount	resq	1
	ccount	resq	1

	char_ans	resb	16

;---------------------------------------------------------------------
global	far_proc		

extern	filehandle, char, buf, abuf_len

%include	"macro.asm"
;---------------------------------------------------------------------
section .text
	global	_main
_main:

far_proc:          		;FAR Procedure
	
		xor	rax,rax
		xor	rbx,rbx
		xor	rcx,rcx
		xor	rsi,rsi	

		mov	bl,[char]
		mov	rsi,buf
		mov	rcx,[abuf_len]

again:	mov	al,[rsi]

case_s:	cmp	al,20h		;space : 32 (20H)
		jne	case_n
		inc	qword[scount]
		jmp	next

case_n:	cmp	al,0Ah		;newline : 10(0AH)
		jne	case_c
		inc	qword[ncount]
		jmp	next

case_c:	cmp	al,bl			;character
		jne	next
		inc	qword[ccount]

next:		inc	rsi
		dec	rcx			;
		jnz	again			;loop again

		print smsg,smsg_len
		mov	rbx,[scount]
		call	display
	
		print nmsg,nmsg_len
		mov	rbx,[ncount]
		call	display

		print cmsg,cmsg_len
		mov	rbx,[ccount]
		call	display

	fclose	[filehandle]
	ret


;----------------------------------------------------------------

display: 
   mov 	edi,char_ans
mov rcx,2
         
again1:   rol bl,4
         mov al,bl
         and al,0fh
         cmp al,9
         jbe x1
         cmp al,'a'
         jb x2
         add al,57h
         jmp x3
x1:     add al,30h
        jmp x3
x2:     add al,37h
x3:     mov[edi],al
        inc edi
        loop again1
       print char_ans,4	
        ret

