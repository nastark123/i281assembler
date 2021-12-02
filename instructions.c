#include "instructions.h"

void parse_noop(char *line, ParsedInstruction *inst) {
    inst->inst = malloc(sizeof(char) * 5);
    strcpy(inst->inst, "NOOP");

    inst->opcode = 0x0000; // noop is always an all-zero opcode 
}

void parse_inputc(char *line, ParsedInstruction *inst) {

}

void parse_inputcf(char *line, ParsedInstruction *inst) {

}

void parse_inputd(char *line, ParsedInstruction *inst) {

}

void parse_inputdf(char *line, ParsedInstruction *inst) {

}

void parse_move(char *line, ParsedInstruction *inst) {

}

void parse_loadi(char *line, ParsedInstruction *inst) {
    char reg[4]; // string that will store the register to be loaded
    uint8_t val; // value to be loaded
    sscanf(line, " %*s %s %d ", reg, &val);

    inst->opcode = val; // assign val to opcode, taking up lower 8 bits
    inst->opcode |= 0x3000; // add the opcode to the upper four bits
    
    
}

void parse_loadp(char *line, ParsedInstruction *inst) {

}

void parse_add(char *line, ParsedInstruction *inst) {

}

void parse_addi(char *line, ParsedInstruction *inst) {

}

void parse_sub(char *line, ParsedInstruction *inst) {

}

void parse_subi(char *line, ParsedInstruction *inst) {

}

void parse_load(char *line, ParsedInstruction *inst) {

}

void parse_loadf(char *line, ParsedInstruction *inst) {

}

void parse_store(char *line, ParsedInstruction *inst) {

}

void parse_storef(char *line, ParsedInstruction *inst) {

}

void parse_shiftl(char *line, ParsedInstruction *inst) {

}

void parse_shiftr(char *line, ParsedInstruction *inst) {

}

void parse_cmp(char *line, ParsedInstruction *inst) {

}

void parse_jump(char *line, ParsedInstruction *inst) {

}

void parse_bre(char *line, ParsedInstruction *inst) {

}

void parse_brz(char *line, ParsedInstruction *inst) {

}

void parse_brne(char *line, ParsedInstruction *inst) {

}

void parse_brnz(char *line, ParsedInstruction *inst) {

}

void parse_brg(char *line, ParsedInstruction *inst) {

}

void parse_brge(char *line, ParsedInstruction *inst) {

}