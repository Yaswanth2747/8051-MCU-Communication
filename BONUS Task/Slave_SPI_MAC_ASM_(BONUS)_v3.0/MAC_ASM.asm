PUBLIC MAC
CSEG AT 0100H

MAC:
    MOV R0, #70H       ; pointer to a1, a2, a3
    MOV R1, #73H       ; pointer to b1, b2, b3

    CLR A
    MOV 51H, A         ; clear MSB
    MOV 52H, A         ; clear LSB
    MOV 50H, A         ; temporary carry storage

    MOV R2, #03H       ; loop counter

MAC_LOOP:
    MOV A, @R0         ; A = ai
    MOV B, @R1         ; B = bi
    MUL AB             ; A*B => A(low), B(high)

    ; Add A (LSB of product) to 52H
    MOV C, 0           ; clear carry
    ADD A, 52H
    MOV 52H, A

    ; Add B (MSB of product) + carry to 51H
    MOV A, B
    ADDC A, 51H
    MOV 51H, A

    INC R0
    INC R1
    DJNZ R2, MAC_LOOP

    RET

END
