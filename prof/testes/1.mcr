SWAP: macro &A,&B
COPY &A,T
COPY &B,&A
COPY T,&B
ENDMACRO

SECAO TEXT
INIT: INPUT N1

INPUT N2
INPUT N3
LOAD N1
SUB N2
JMPN PROX

swap N1,N2

PROX: LOAD N1
SUB N3
JMPN ULT

SWAP N1,N3

ULT: LOAD N2
SUB N3
JMPN OUT
SWAP N2,N3

OUT: OUTPUT N1
OUTPUT N2
OUTPUT N3
JMPP FIM

FIM: INPUT N4
LOAD n4
JMPP INIT
JMPZ INIT
STOP


SECAO DATA
N1: SPACE
N2: SPACE
N3: SPACE
N4:SPACE
T: SPACE