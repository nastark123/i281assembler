#include "instructions.h"

bool parse_noop(char *line, int line_num, ParsedInstruction *inst) {
    inst->inst = malloc(sizeof(char) * 5);
    strcpy(inst->inst, "NOOP");

    inst->opcode = 0x0000; // noop is always an all-zero opcode

    printf("0x0000\n");

    return true;
}

bool parse_inputc(char *line, int line_num, ParsedInstruction *inst) {
    printf("0xFFFF\n");
}

bool parse_inputcf(char *line, int line_num, ParsedInstruction *inst) {
    printf("0xFFFF\n");
}

bool parse_inputd(char *line, int line_num, ParsedInstruction *inst) {
    printf("0xFFFF\n");
}

bool parse_inputdf(char *line, int line_num, ParsedInstruction *inst) {
    printf("0xFFFF\n");
}

bool parse_move(char *line, int line_num, ParsedInstruction *inst) {
    char reg0, reg1; // characters for each of the registers to be added
    sscanf(line, "%*s %c , %c ", &reg0, &reg1);

    if(reg0 > 'D' || reg0 < 'A') {
        printf("Invalid register 0 \"%c\" specified for MOVE instruction on line %d, compilation aborting...\n", reg0, line_num);
        return false;
    }
    if(reg1 > 'D' || reg1 < 'A') {
        printf("Invalid register 1 \"%c\" specified for MOVE instruction on line %d, compilation aborting...\n", reg1, line_num);
        return false;
    }

    inst->opcode = 0x2000;
    inst->opcode |= (reg0 - 'A') << 10;
    inst->opcode |= (reg1 - 'A') << 8;

    printf("0x%04x\n", inst->opcode);

    return true;
}

bool parse_loadi(char *line, int line_num, ParsedInstruction *inst) {
    char reg; // character that will store the register to be loaded
    uint8_t val; // value to be loaded
    sscanf(line, " %*s %c , %hhu ", &reg, &val);

    if(reg > 'D' || reg < 'A') {
        printf("Invalid register \"%c\" specified for LOADI instruction on line %d, compilation aborting...\n", reg, line_num);
        exit(-1);
    }

    inst->opcode = val; // assign val to opcode, taking up lower 8 bits
    inst->opcode |= 0x3000; // add the opcode to the upper four bits
    inst->opcode |= (reg  - 'A') << 10;

    printf("0x%04x\n", inst->opcode);

    return true;    
}

bool parse_loadp(char *line, int line_num, ParsedInstruction *inst) {
    char reg; // character that will store the register to be loaded
    uint8_t val; // value to be loaded
    sscanf(line, " %*s %c , %hhu ", &reg, &val);

    if(reg > 'D' || reg < 'A') {
        printf("Invalid register \"%c\" specified for LOADP instruction on line %d, compilation aborting...\n", reg, line_num);
        return false;
    }

    inst->opcode = val; // assign val to opcode, taking up lower 8 bits
    inst->opcode |= 0x3000; // add the opcode to the upper four bits
    inst->opcode |= (reg  - 'A') << 10;

    printf("0x%04x\n", inst->opcode);

    return true;
}

bool parse_add(char *line, int line_num, ParsedInstruction *inst) {
    char reg0, reg1; // characters for each of the registers to be added
    sscanf(line, "%*s %c , %c ", &reg0, &reg1);

    if(reg0 > 'D' || reg0 < 'A') {
        printf("Invalid register 0 \"%c\" specified for ADD instruction on line %d, compilation aborting...\n", reg0, line_num);
        return false;
    }
    if(reg1 > 'D' || reg1 < 'A') {
        printf("Invalid register 1 \"%c\" specified for ADD instruction on line %d, compilation aborting...\n", reg1, line_num);
        return false;
    }

    inst->opcode = 0x4000;
    inst->opcode |= (reg0 - 'A') << 10;
    inst->opcode |= (reg1 - 'A') << 8;

    printf("0x%04x\n", inst->opcode);

    return true;
}

bool parse_addi(char *line, int line_num, ParsedInstruction *inst) {
    char reg; // characters for each of the registers to be added
    uint8_t val; // immediate value
    sscanf(line, "%*s %c , %hhu ", &reg, &val);

    if(reg > 'D' || reg < 'A') {
        printf("Invalid register \"%c\" specified for ADDI instruction on line %d, compilation aborting...\n", reg, line_num);
        return false;
    }

    inst->opcode = val;
    inst->opcode |= 0x5000;
    inst->opcode |= (reg - 'A') << 10;

    printf("0x%04x\n", inst->opcode);

    return true;
}

bool parse_sub(char *line, int line_num, ParsedInstruction *inst) {
    char reg0, reg1; // characters for each of the registers to be added
    sscanf(line, "%*s %c , %c ", &reg0, &reg1);

    if(reg0 > 'D' || reg0 < 'A') {
        printf("Invalid register 0 \"%c\" specified for SUB instruction on line %d, compilation aborting...\n", reg0, line_num);
        return false;
    }
    if(reg1 > 'D' || reg1 < 'A') {
        printf("Invalid register 1 \"%c\" specified for SUB instruction on line %d, compilation aborting...\n", reg1, line_num);
        return false;
    }

    inst->opcode = 0x6000;
    inst->opcode |= (reg0 - 'A') << 10;
    inst->opcode |= (reg1 - 'A') << 8;

    printf("0x%04x\n", inst->opcode);

    return true;
}

bool parse_subi(char *line, int line_num, ParsedInstruction *inst) {
    char reg; // characters for each of the registers to be added
    uint8_t val; // immediate value
    sscanf(line, "%*s %c , %hhu ", &reg, &val);

    if(reg > 'D' || reg < 'A') {
        printf("Invalid register \"%c\" specified for SUBI instruction on line %d, compilation aborting...\n", reg, line_num);
        return false;
    }

    inst->opcode = val;
    inst->opcode |= 0x7000;
    inst->opcode |= (reg - 'A') << 10;

    printf("0x%04x\n", inst->opcode);

    return true;
}

bool parse_load(char *line, int line_num, ParsedInstruction *inst) {
    printf("0xFFFF\n");
}

bool parse_loadf(char *line, int line_num, ParsedInstruction *inst) {
    printf("0xFFFF\n");
}

bool parse_store(char *line, int line_num, ParsedInstruction *inst) {
    printf("0xFFFF\n");
}

bool parse_storef(char *line, int line_num, ParsedInstruction *inst) {
    printf("0xFFFF\n");
}

bool parse_shiftl(char *line, int line_num, ParsedInstruction *inst) {
    char reg; // characters for each of the registers to be added
    sscanf(line, "%*s %c ", &reg);

    if(reg > 'D' || reg < 'A') {
        printf("Invalid register \"%c\" specified for SHIFTL instruction on line %d, compilation aborting...\n", reg, line_num);
        return false;
    }

    inst->opcode = 0xC000;
    inst->opcode |= (reg - 'A') << 10;

    printf("0x%04x\n", inst->opcode);

    return true;
}

bool parse_shiftr(char *line, int line_num, ParsedInstruction *inst) {
    char reg; // characters for each of the registers to be added
    sscanf(line, "%*s %c ", &reg);

    if(reg > 'D' || reg < 'A') {
        printf("Invalid register \"%c\" specified for SHIFTR instruction on line %d, compilation aborting...\n", reg, line_num);
        return false;
    }

    inst->opcode = 0xC100;
    inst->opcode |= (reg - 'A') << 10;

    printf("0x%04x\n", inst->opcode);

    return true;
}

bool parse_cmp(char *line, int line_num, ParsedInstruction *inst) {
    printf("0xFFFF\n");
}

bool parse_jump(char *line, int line_num, ParsedInstruction *inst) {
    printf("0xFFFF\n");
}

bool parse_bre(char *line, int line_num, ParsedInstruction *inst) {
    printf("0xFFFF\n");
}

bool parse_brz(char *line, int line_num, ParsedInstruction *inst) {
    printf("0xFFFF\n");
}

bool parse_brne(char *line, int line_num, ParsedInstruction *inst) {
    printf("0xFFFF\n");
}

bool parse_brnz(char *line, int line_num, ParsedInstruction *inst) {
    printf("0xFFFF\n");
}

bool parse_brg(char *line, int line_num, ParsedInstruction *inst) {
    printf("0xFFFF\n");
}

bool parse_brge(char *line, int line_num, ParsedInstruction *inst) {
    printf("0xFFFF\n");
}