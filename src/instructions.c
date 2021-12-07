#include "instructions.h"

void add_inst_name(ParsedInstruction *inst, char *name) {
    inst->inst = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(inst->inst, name);
}

bool check_regs(char reg) {
    if(reg < MIN_REG || reg > MAX_REG) return false;
    return true;
}

bool parse_noop(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "NOOP");

    inst->opcode = 0x0000; // noop is always an all-zero opcode

    // printf("%d: 0x0000\n", line_num);

    return true;
}

bool parse_inputc(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "INPUTC");

    uint8_t caddress;
    if(sscanf(line, " %*s %hhu ", &caddress) < 1) {
        printf("Missing code address for INPUTC instruction on line %d, compilation aborting...\n", line_num);
        return false;
    }

    inst->opcode = caddress;
    inst->opcode |= 0x1000;

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_inputcf(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "INPUTCF");

    char reg;
    uint8_t caddress;
    if(sscanf(line, " %*s %c , %hhu ", &reg, &caddress) < 2) {
        printf("Missing register or code address for INPUTCF instruction on line %d, compilation aborting...\n", line_num);
        return false;
    }

    if(!check_regs(reg)) {
        printf("Invalid register \"%c\" specified for INPUTCF instruction on line %d, compilation aborting...\n", reg, line_num);
        return false;
    }

    inst->opcode = caddress;
    inst->opcode |= 0x1100;
    inst->opcode |= (reg - 'A') << 10;

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_inputd(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "INPUTD");

    uint8_t daddress;
    if(sscanf(line, " %*s %hhu ", &daddress) < 1) {
        printf("Missing data address for INPUTD instruction on line %d, compilation aborting...\n", line_num);
        return false;
    }

    inst->opcode = daddress;
    inst->opcode |= 0x1200;

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_inputdf(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "INPUTDF");

    char reg;
    uint8_t daddress;
    if(sscanf(line, " %*s %c , %hhu ", &reg, &daddress) < 2) {
        printf("Missing register or code address for INPUTDF instruction on line %d, compilation aborting...\n", line_num);
        return false;
    }

    if(!check_regs(reg)) {
        printf("Invalid register \"%c\" specified for INPUTDF instruction on line %d, compilation aborting...\n", reg, line_num);
        return false;
    }

    inst->opcode = daddress;
    inst->opcode |= 0x1300;
    inst->opcode |= (reg - 'A') << 10;

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_move(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "MOVE");

    char reg0, reg1; // characters for each of the registers to be added
    if(sscanf(line, "%*s %c , %c ", &reg0, &reg1) < 2) {
        printf("Missing one or more registers for MOVE instruction on line %d, compilation aborting...\n", line_num);
        return false;
    }

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

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_loadi(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "LOADI");

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

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;    
}

bool parse_loadp(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "LOADP");

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

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_add(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "ADD");

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

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_addi(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "ADDI");

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

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_sub(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "SUB");

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

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_subi(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "SUBI");

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

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_load(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "LOAD");

    char reg;
    char sym = -1;
    uint8_t daddress = 0;
    uint8_t offset = 0;
    sscanf(line, " %*s %*c, [ %*u %c", &sym);
    if(sym == ']') {
        if(sscanf(line, " %*s %c , [ %hhu ] ", &reg, &daddress) < 2) {
            printf("Missing register or data address for LOAD instruction on line %d, compilation aborting...\n", line_num);
            return false;
        }
        sym = '+';
    } else {
        if(sscanf(line, " %*s %c , [ %hhu %c %hhu ]", &reg, &daddress, &sym, &offset) < 4) {
            printf("Missing register or data address for LOAD instruction on line %d, compilation aborting...\n", line_num);
            return false;
        }
    }

    if(!check_regs(reg)) {
        printf("Invalid register \"%c\" specified for LOAD instruction on line %d, compilation aborting...\n", reg, line_num);
        return false;
    }

    if(sym == '+') {
        daddress += offset;
    } else if(sym == '-') {
        daddress -= offset;
    } else {
        printf("Invalid operation %c specified for LOAD instruction address on line %d, compilation aborting...\n", sym, line_num);
        return false;
    }

    inst->opcode = daddress;
    inst->opcode |= 0x8000;
    inst->opcode |= (reg - 'A') << 10;

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_loadf(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "LOADF");

    char reg0, reg1;
    char sym0 = -1;
    char sym1 = -1;
    uint8_t daddress, offset;
    if(sscanf(line, " %*s %c , [ %hhu %c %c %c %hhu ] ", &reg0, &daddress, &sym0, &reg1, &sym1, &offset) < 3) { // check for three since it is possible to have a missing offset and still be valid
        printf("Missing one or more registers or data address for LOADF instruction on line %d, compilation aborting...\n", line_num);
        return false;
    }

    if(!check_regs(reg0)) {
        printf("Invalid register 0 \"%c\" specified for LOADF instruction on line %d, compilation aborting...\n", reg0, line_num);
        return false;
    }

    if(!check_regs(reg1)) {
        printf("Invalid register 1 \"%c\" specified for LOADF instruction on line %d, compilation aborting...\n", reg1, line_num);
        return false;
    }

    if(sym0 != '+') {
        printf("Invalid operation %c specified for LOADF instruction address on line %d, must be \"+\", compilation aborting...\n", sym0, line_num);
        return false;
    }

    if(sym1 == '+') {
        daddress += offset;
    } else if(sym1 == '-') {
        daddress -= offset;
    } else if(sym1 != ']') { // the trailing bracket will be picked up by the second symbol character if no offset is specified
        printf("Invalid operation %c specified for LOADF instruction address on line %d, compilation aborting...\n", sym1, line_num);
        return false;
    }

    inst->opcode = daddress;
    inst->opcode |= 0x9000;
    inst->opcode |= (reg0 - 'A') << 10;
    inst->opcode |= (reg1 - 'A') << 8;

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_store(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "STORE");

    char reg;
    char sym = -1;
    uint8_t daddress = 0;
    uint8_t offset = 0;
    sscanf(line, "%*s [ %*u %c", &sym);
    if(sym == ']') {
        if(sscanf(line, " %*s [ %hhu ], %c ", &daddress, &reg) < 2) {
            printf("Missing register or data address for STORE instruction on line %d, compilation aborting...\n", line_num);
            return false;
        }
        sym = '+'; // prevents errors later
    } else {
        if(sscanf(line, " %*s [ %hhu %c %hhu ], %c ", &daddress, &sym, &offset, &reg) < 4) {
            printf("Missing register or data address for STORE instruction on line %d, compilation aborting...\n", line_num);
            return false;
        }
    }

    if(!check_regs(reg)) {
        printf("Invalid register \"%c\" specified for STORE instruction on line %d, compilation aborting...\n", reg, line_num);
        return false;
    }

    if(sym == '+') {
        daddress += offset;
    } else if(sym == '-') {
        daddress -= offset;
    } else {
        printf("Invalid operation %c specified for STORE instruction address on line %d, compilation aborting...\n", sym, line_num);
        return false;
    }

    inst->opcode = daddress;
    inst->opcode |= 0xA000;
    inst->opcode |= (reg - 'A') << 10;

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_storef(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "STOREF");

    char reg0, reg1;
    char sym0 = -1;
    char sym1 = -1;
    uint8_t daddress = 0;
    uint8_t offset = 0;

    // this is really hacky but I'm not sure how to make it any better
    sscanf(line, " %*s [ %*u %*c %*c %c ", &sym1);
    if(sym1 == ']') {
        if(sscanf(line, " %*s [ %hhu %c %c ] , %c ", &daddress, &sym0, &reg1, &reg0) < 3) { // check for three since it is possible to have a missing offset and still be valid
            printf("Missing one or more registers or data address for STOREF instruction on line %d, compilation aborting...\n", line_num);
            return false;
        }
        sym1 = '+'; // to prevent errors later
    } else {
        if(sscanf(line, " %*s [ %hhu %c %c %c %hhu ] , %c ", &daddress, &sym0, &reg1, &sym1, &offset, &reg0) < 3) { // check for three since it is possible to have a missing offset and still be valid
            printf("Missing one or more registers or data address for STOREF instruction on line %d, compilation aborting...\n", line_num);
            return false;
        }
    }

    if(!check_regs(reg0)) {
        printf("Invalid register 0 \"%c\" specified for STOREF instruction on line %d, compilation aborting...\n", reg0, line_num);
        return false;
    }

    if(!check_regs(reg1)) {
        printf("Invalid register 1 \"%c\" specified for STOREF instruction on line %d, compilation aborting...\n", reg1, line_num);
        return false;
    }

    if(sym0 != '+') {
        printf("Invalid operation %c specified for STOREF instruction address on line %d, must be \"+\", compilation aborting...\n", sym0, line_num);
        return false;
    }

    if(sym1 == '+') {
        daddress += offset;
    } else if(sym1 == '-') {
        daddress -= offset;
    } else {
        printf("Invalid operation %c specified for STOREF instruction address on line %d, compilation aborting...\n", sym1, line_num);
        return false;
    }

    inst->opcode = daddress;
    inst->opcode |= 0xB000;
    inst->opcode |= (reg0 - 'A') << 10;
    inst->opcode |= (reg1 - 'A') << 8;

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_shiftl(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "SHIFTL");

    char reg; // characters for each of the registers to be added
    sscanf(line, "%*s %c ", &reg);

    if(reg > 'D' || reg < 'A') {
        printf("Invalid register \"%c\" specified for SHIFTL instruction on line %d, compilation aborting...\n", reg, line_num);
        return false;
    }

    inst->opcode = 0xC000;
    inst->opcode |= (reg - 'A') << 10;

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_shiftr(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "SHIFTR");

    char reg; // characters for each of the registers to be added
    sscanf(line, "%*s %c ", &reg);

    if(reg > 'D' || reg < 'A') {
        printf("Invalid register \"%c\" specified for SHIFTR instruction on line %d, compilation aborting...\n", reg, line_num);
        return false;
    }

    inst->opcode = 0xC100;
    inst->opcode |= (reg - 'A') << 10;

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_cmp(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "CMP");

    char reg0, reg1; // characters for each of the registers to be added
    sscanf(line, "%*s %c , %c ", &reg0, &reg1);

    if(!check_regs(reg0)) {
        printf("Invalid register 0 \"%c\" specified for CMP instruction on line %d, compilation aborting...\n", reg0, line_num);
        return false;
    }
    if(!check_regs(reg1)) {
        printf("Invalid register 1 \"%c\" specified for CMP instruction on line %d, compilation aborting...\n", reg1, line_num);
        return false;
    }

    inst->opcode = 0xD000;
    inst->opcode |= (reg0 - 'A') << 10;
    inst->opcode |= (reg1 - 'A') << 8;

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_jump(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "JUMP");

    uint8_t pcoffset;
    if(sscanf(line, " %*s %hhu ", &pcoffset) < 1) {
        printf("Missing label for JUMP instruction on line %d, compilation aborting...\n", line_num);
        return false;
    }

    inst->opcode = pcoffset;
    inst->opcode |= 0xE000;

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_bre(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "BRE");

    uint8_t pcoffset;
    if(sscanf(line, " %*s %hhu ", &pcoffset) < 1) {
        printf("Missing label for BRE instruction on line %d, compilation aborting...\n", line_num);
        return false;
    }

    inst->opcode = pcoffset;
    inst->opcode |= 0xF000;

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_brz(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "BRZ");

    uint8_t pcoffset;
    if(sscanf(line, " %*s %hhu ", &pcoffset) < 1) {
        printf("Missing label for BRZ instruction on line %d, compilation aborting...\n", line_num);
        return false;
    }

    inst->opcode = pcoffset;
    inst->opcode |= 0xF000;

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_brne(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "BRNE");

    uint8_t pcoffset;
    if(sscanf(line, " %*s %hhu ", &pcoffset) < 1) {
        printf("Missing label for BRNE instruction on line %d, compilation aborting...\n", line_num);
        return false;
    }

    inst->opcode = pcoffset;
    inst->opcode |= 0xF100;

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_brnz(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "BRNZ");

    uint8_t pcoffset;
    if(sscanf(line, " %*s %hhu ", &pcoffset) < 1) {
        printf("Missing label for BRNZ instruction on line %d, compilation aborting...\n", line_num);
        return false;
    }

    inst->opcode = pcoffset;
    inst->opcode |= 0xF100;

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_brg(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "BRG");

    uint8_t pcoffset;
    if(sscanf(line, " %*s %hhu ", &pcoffset) < 1) {
        printf("Missing label for BRG instruction on line %d, compilation aborting...\n", line_num);
        return false;
    }

    inst->opcode = pcoffset;
    inst->opcode |= 0xF200;

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}

bool parse_brge(char *line, int line_num, ParsedInstruction *inst) {
    add_inst_name(inst, "BRGE");

    uint8_t pcoffset;
    if(sscanf(line, " %*s %hhu ", &pcoffset) < 1) {
        printf("Missing label for BRGE instruction on line %d, compilation aborting...\n", line_num);
        return false;
    }

    inst->opcode = pcoffset;
    inst->opcode |= 0xF300;

    // printf("%d: 0x%04x\n", line_num, inst->opcode);

    return true;
}