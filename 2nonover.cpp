section .data
	
	src_blk db 10,'Source block contents are:01 02 03 04 05',10
	src_len equ $-src_blk

        dst_blk db 10,' Dest. bolk contents atfter xfer are:'
	dst_len equ $-dst_blk
	
	msg db 10,'*********MENU************'
	msg_len equ $-msg

	m1 db 10,'1.NOn overlapped without string instructions'
	m1_len equ $-m1

	m2 db 10,'2.NOn overlapped with string instructions'
	m2_len equ $-m2

	m3 db 10,'3.EXIT'
	m3_len equ $-m3

	m4 db 10,'Enter your choice:'
	m4_len equ $-m4
	
array db 01h,02h,03,04h,05h
newarray times 5 db 0
cnt equ 05
colon db ':'
nwline db 10


section .bss
choice resb 02
dispbuff resb 08
	
%macro dispmsg 2
	mov eax,04
	mov ebx,01
	mov ecx,%1
        mov edx,%2
	int 80h
%endmacro

%macro accept 2
	mov eax,03
	mov ebx,0
	mov ecx,%1
	mov edx,%2
	int 80h
%endmacro

section .text
global _start
_start:
	menu:	
	
	dispmsg msg,msg_len
	dispmsg m1,m1_len
	dispmsg m2,m2_len
	dispmsg m3,m3_len
	dispmsg src_blk,src_len
	dispmsg m4,m4_len

	accept choice,02
	cmp byte [choice],'1'
	je wo

	cmp byte [choice],'2'
	je with
cmp byte [choice],'3'
je exit

  exit:      mov eax,01
	mov ebx,00
	int 80h

with:	
	dispmsg dst_blk,dst_len
	dispmsg nwline,1
	mov esi,array
	mov edi,newarray
	mov ecx,cnt
	cld
	rep movsb

	mov esi,array
	xor rcx,rcx
	mov rcx,10

	up2: 
	push rcx
	mov ebx,esi
	call disp8
	dispmsg colon,1
	mov bl,[esi]
	call disp_proc
	inc esi
	;dispmsg spacechar,1
	pop rcx
	loop up2
	jmp menu


wo:	dispmsg dst_blk,dst_len
	dispmsg nwline,1
	mov esi,array
	mov edi,newarray
	mov ecx,cnt
q:	mov al,[esi]
	mov [edi],al
	inc esi
	inc edi
	loop q
	mov esi,array
	xor rcx,rcx
	mov rcx,10

	up3: 
	push rcx
	mov ebx,esi
	call disp8
	dispmsg colon,1
	mov bl,[esi]
	call disp_proc
	inc esi
	;dispmsg spacechar,1
	pop rcx
	loop up3
	jmp menu
	
disp_proc:
	mov ecx,2
	mov edi,dispbuff
dup1:
	rol bl,4
	mov al,bl
	and al,0fh
	cmp al,09
	jbe dskip
	add al,07h
dskip:	add al,30h
	mov [edi],al
	inc edi
	loop dup1

	dispmsg dispbuff,2
	dispmsg nwline,1
	ret
disp8:
	mov ecx,8
	mov edi,dispbuff
dup2:
	rol ebx,4
	mov al,bl
	and al,0fh
	cmp al,09
	jbe dskip2
	add al,07h
dskip2:	add al,30h
	mov [edi],al
	inc edi
	loop dup2

	dispmsg dispbuff,8
	ret





