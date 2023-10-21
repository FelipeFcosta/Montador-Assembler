INPUT N
LOAD N
div tres
mul tres
sub n
jmpz sim
jmp nao
SIM: OUTPUT UM
jmp fim
NAO:    OUTPUT ZERO
FIM: STOP
UM: CONST 1
ZERO: CONST 0
TRES: CONST 3
N: SPACE