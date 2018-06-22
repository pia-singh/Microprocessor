CODE SEGMENT
 ASSUME CS:CODE,DS:CODE

MAIN : JMP INIT
       OLD_ADD DD ?
       NO DB 0
	count dw 00h

OUR_ISR: PUSH AX
        PUSH BX
        PUSH CX
        PUSH DX
        PUSH DS
        PUSH SS
        PUSH ES
        PUSH DI
        PUSH SI

        MOV AX,CS
        MOV DS,AX

  	mov cx,count
        inc cx
        mov count,cx
        cmp cx,100
        
          jne nobeep

        mov cx,0000h
        mov count,cx
        call time
 call tone
jmp nobeep

       
   time proc
    MOV AH,02           ;GET TIME
        INT 1AH
                ;IT Return TIME IN CH REG.: HRS
                ;         IN CL REG.: MIN
                ;         IN DL REG.: SEC
       
        MOV AX,0B800H
        MOV ES,AX
        MOV SI,400

        MOV NO,CH
        CALL DISP
        CALL COLON
        MOV NO,CL
        CALL DISP
        CALL COLON
        MOV NO,DH
        CALL DISP
ret 
time endp
		



 nobeep:  POP SI
        POP DI
        POP ES
        POP SS
        POP DS
        POP DX
        POP CX
        POP BX
        POP AX
     JMP CS:OLD_ADD


tone proc 

	   mov     al, 182         ; Prepare the speaker for the
        out     43h, al         ;  note.
        mov     ax, 4560        ; Frequency number (in decimal)
                                ;  for middle C.
        out     42h, al         ; Output low byte.
        mov     al, ah          ; Output high byte.
        out     42h, al 
        in      al, 61h         ; Turn on note (get value from
                                ;  port 61h).
        or      al, 00000011b   ; Set bits 1 and 0.
        out     61h, al         ; Send new value.
        mov     bx, 25          ; Pause for duration of note.
.pause1:
        mov     cx, 65535
.pause2:
        dec     cx
        jne     .pause2
        dec     bx
        jne     .pause1
        in      al, 61h         ; Turn off note (get value from
                                ;  port 61h).
        and     al, 11111100b   ; Reset bits 1 and 0.
        out     61h, al         ; Send new value.	
	   ret
	tone endp
	


	

     
DISP PROC 
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX

    MOV CH,2
    MOV BL,NO

DISP1:  MOV CL,4
       ROR BL,CL
       MOV AL,BL
       AND AL,0FH
       ADD AL,30H
       MOV  ES:[SI],AL
       INC SI
       MOV AL,10100001B
       MOV ES:[SI],AL
       INC SI
       DEC CH
       JNZ DISP1
       POP DX
       POP CX
       POP BX
       POP AX
       RET
       DISP ENDP

COLON PROC
        MOV AL,':'
        MOV ES:[SI],AL
        INC SI
        MOV AL,10010011B
        MOV ES:[SI],AL
        INC SI
        RET
        COLON ENDP



INIT: CLI
        MOV AX,CS
        MOV DS,AX

        MOV AH,35H
        MOV AL,8
        INT 21H

MOV WORD PTR OLD_ADD+2,ES
MOV WORD PTR OLD_ADD,BX

        MOV AH,25H
        MOV AL,8
        LEA DX,OUR_ISR
        INT 21H

        MOV AH,31H
        MOV AL,1
        LEA DX,INIT
        INT 21H
        STI
        CODE ENDS
        END MAIN


