; This is a program with errors

; 1 - ERROR in err1.am line 4 - label can't be a register
r3: add r3,r15 
; 2 - ERROR in err1.am line 6 - label can't be a command name
add: add r3,r15 
; 3 - ERROR in err1.am line 8 - character "#" is not allowed in label
LAB#EL1: move r5,r6
; 4 - ERROR in err1.am line 10 - after a label must be a space
LABEL1:move r5,r6
; 5 - ERROR in err1.am line 12 - Illegal command "move"
move r1
; 6 - ERROR in err1.am line 14 - 2 operands are needed in command "mov"
mov r1
; 7 - ERROR in err1.am line 16 - extra char "," after second operand
mov r1,r1,
; 8 - ERROR in err1.am line 18 - Can't evaluate opernd(s) in this line
mov r1,,r1
; 9 - ERROR in err1.am line 20 - Expecting a number
mov r1,#r
; 10 - ERROR in err1.am line 22 - immidiate "10000000" exceed 16 bits
mov r1,#10000000
; 11 - ERROR in err1.am line 24 - wrong dest add mode for mov command
mov r1, #2
; 12 - ERROR in err1.am line 26 - wrong source add mode for lea command
lea r3,r3
; 13 - ERROR in err1.am line 28 - 1 operands is needed in command "clr"
 clr r1,r2
; 14 - ERROR in err1.am line 31 - label A can't be .extern and .entry together
.entry A
.extern A
; 15 - ERROR in err1.am line 33 - Register must be between 10 to 15
mov r3,A[r1]
; 16 - ERROR in err1.am line 35 - Expecting a number
mov r3,A[rrrr]
; 17 - ERROR in err1.am line 37 - Add Mode Index must have ] after the reg
mov r3,A[r12A]
; 18 - ERROR in err1.am line 40 - label LABELNAMETOOLONG1111111111111111  
;                                   has more charecters than allowed
LABELNAMETOOLONG1111111111111111: mov r1,r1
; 19 - ERROR in err1.am line 43 - label LABELNAMETOOLONG1111111111111111  
;                                   has more charecters than allowed
mov LABELNAMETOOLONG1111111111111111,r1
; 20 - ERROR in err1.am line 45 - after .data must be space or tab
.data1,2,3
; 21 - ERROR in err1.am line 47 - The string must be inside ""
.string abc
; 22 - ERROR in err1.am line 50 - double declaration of label A
A: mov r1,r1
A: mov r1,r1
; 23 - ERROR in err1.am line 53 - .extern can't be also a label B in this file
 B: mov r1,r1
.extern B
; 24 - ERROR in err1.am line 56 - label C can't be also .extern in this file 
.extern C
 C: mov r1,r1
; 25 - ERROR in err1.am line 58 - label without data or code is not allowed
LABELWITHOUTCODE:
; 26 - ERROR in err1.am line 60 - command "stop" doesn't have any operands
stop r1
; 27 - ERROR in err1.am line 62 - unrecognized char "d" after #5
mov #5d,r1
; 28 - ERROR in err1.am line 64 - unrecognized char "e" after r5
mov r5 e,r1
; 29 - ERROR in err1.am line 66 - must be a register in the [] brackets
mov r1[123],r2
; 30 -  ERROR in err1.am line 68 - label must begin with an alphanumeric char
12LABEL: mov r1,r1
; 31 - ERROR in err1.am line 70 - string must end with "
.string "1234
; 31 - ERROR in err1.am line 72 - string must end with "
.string "1234" k
; 33 - ERROR in err1.am line 74 - no numbers given
.data
; 34 - ERROR in err1.am line 76 - no string given
.string
; 35 - ERROR in err1.am line 78 - Missing a number
.data 1,2,3,,4
; 36 - ERROR in err1.am line 80 - missing comma after number 7
.data  5,6,7 8
; 37 - ERROR in err1.am line 82 - Can't evaluate Label
.entry 123a
; 38 - ERROR in err1.am line 84 - extra char "D" after label in .extern command
 .extern ABC D
; 39 - ERROR in err1.am line 86 - char � is not printible
.string "א"

