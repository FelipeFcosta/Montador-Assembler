SECAO TEXT
INPUT LIMIT
COPY ZERO,PREV
COPY ONE,CURR
LOAD PREV
FIB: OUTPUT CURR
LOAD CURR
ADD PREV
STORE NEW
SUB LIMIT
JMPP FINAL
COPY CURR,PREV
COPY NEW,CURR
JMP FIB
FINAL: STOP


SECAO DATA

ZERO: CONST 0
ONE: CONST 1
LIMIT: SPACE
PREV: SPACE
CURR: SPACE
NEW: SPACE
