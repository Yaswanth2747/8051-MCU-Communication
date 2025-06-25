; add.asm
; Adds values from 0x30 and 0x31, stores result in 0x32

PUBLIC ADD_ASM
;NAME ADD_FILE

CSEG AT 0100H    ; ?? Place code starting from 0x100 (safe area)

ADD_ASM:
    MOV A, 30H
    ADD A, 31H
    MOV 32H, A
    RET

END
