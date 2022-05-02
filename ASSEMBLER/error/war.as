; This is a program with warnings

; 1 
.entry A
; 2 - Warning in war.am line 6 - .entry is already declared for label A
.entry A
; 3 - Warning in war.am line 9 - .extern is already declared for label B
.extern B
.extern B
; 4- Warning in war.as line 13 - macro m1 has no lines
macro m1
endm
m1
A: mov r1, r1 


