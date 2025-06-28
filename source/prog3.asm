MOV 100, R0
MOV 100, R1
ADD R0, R1    ; Should set OF (200 > 127)
MOV -100, R2
MOV -100, R3
ADD R2, R3    ; Should set UF (-200 < -128)
MOV 5, R4
MOV -5, R5
ADD R4, R5    ; Should set ZF (0)