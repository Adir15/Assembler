commandConst cc = { 
   { /* commandNames */
    "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", 
    "jsr", "red", "prn", "rts", "stop"},
   {/* commandOpcode */ 
     0, /* mov */
     1, /* cmp */
     2, /* add */
     2, /* sub */
     4, /* lea */
     5, /* clr */
     5, /* not */
     5, /* inc */
     5, /* dec */
     9, /* jmp */
     9, /* bne */
     9, /* jsr */
    12, /* red */
    13, /* prn */
    14, /* rts */
    15},/* stop */
   {/* commandFunct */    
     0, /* mov */
     0, /* cmp */
    10, /* add */
    11, /* sub */
     0, /* lea */
    10, /* clr */
    11, /* not */
    12, /* inc */
    13, /* dec */
    10, /* jmp */
    11, /* bne */
    12, /* jsr */
     0, /* red */
     0, /* prn */
     0, /* rts */
     0},/* stop */
   {/* commandICinc */
    2, /* mov */
    2, /* cmp */
    2, /* add */
    2, /* sub */
    2, /* lea */
    2, /* clr */
    2, /* not */
    2, /* inc */
    2, /* dec */
    2, /* jmp */
    2, /* bne */
    2, /* jsr */
    2, /* red */
    2, /* prn */
    1, /* rts */
    1},/* stop */
   { /* commandOperands */
    2, /* mov */
    2, /* cmp */
    2, /* add */
    2, /* sub */
    2, /* lea */
    1, /* clr */
    1, /* not */
    1, /* inc */
    1, /* dec */
    1, /* jmp */
    1, /* bne */
    1, /* jsr */
    1, /* red */
    1, /* prn */
    0, /* rts */
    0},/* stop */
/*
 * 0->#imm; 1->Label; 2->Label[R]; 3->R
 */
   { /* commandSrcAdressingMode */
    0,1,2,3,     /* mov */
    0,1,2,3,     /* cmp */
    0,1,2,3,     /* add */
    0,1,2,3,     /* sub */
    1,2,2,2,     /* lea */
    -1,-1,-1,-1, /* clr */
    -1,-1,-1,-1, /* not */
    -1,-1,-1,-1, /* inc */
    -1,-1,-1,-1, /* dec */
    -1,-1,-1,-1, /* jmp */
    -1,-1,-1,-1, /* bne */
    -1,-1,-1,-1, /* jsr */
    -1,-1,-1,-1, /* red */
    -1,-1,-1,-1, /* prn */
    -1,-1,-1,-1, /* rts */
    -1,-1,-1,-1},/* stop */
   { /* commandDestAdressingMode */
    1,2,3,3,     /* mov */
    0,1,2,3,     /* cmp */
    1,2,3,3,     /* add */
    1,2,3,3,     /* sub */
    1,2,3,3,     /* lea */
    1,2,3,3, 	 /* clr */
    1,2,3,3, 	 /* not */
    1,2,3,3, 	 /* inc */
    1,2,3,3, 	 /* dec */
    1,2,2,2, 	 /* jmp */
    1,2,2,2, 	 /* bne */
    1,2,2,2, 	 /* jsr */
    1,2,3,3, 	 /* red */
    0,1,2,3, 	 /* prn */
    -1,-1,-1,-1, /* rts */
    -1,-1,-1,-1} /* stop */
};




