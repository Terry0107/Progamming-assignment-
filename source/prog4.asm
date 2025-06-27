MOV 88, R0         ; R0 = 88
MOV 10, R1         ; R1 = 10
STORE R0, [R1]     ; MEM[10] = 88
LOAD [R1], R2      ; R2 = MEM[10] -> 88
DISPLAY R2         ; Output R2 (should be 88)

STORE R2, 20       ; MEM[20] = R2
LOAD 20, R3        ; R3 = MEM[20]
DISPLAY R3         ; Output R3 (should be 88)