typedef struct commandConst {
    char commandNames[NUM_COMMANDS][5];
    int  commandOpcode[NUM_COMMANDS];
    int  commandFunct[NUM_COMMANDS];
    int  commandICinc[NUM_COMMANDS];
    int  commandOperands[NUM_COMMANDS];
    int  commandSrcAdressingMode[NUM_COMMANDS*4];
    int  commandDestAdressingMode[NUM_COMMANDS*4];
} commandConst;

