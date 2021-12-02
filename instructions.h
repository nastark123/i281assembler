#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>
#include <string.h>

/**
 * This file contains function declarations for each of the assembly instructions so that they can be parsed properly. 
 */

typedef struct {
    char *inst;
    uint16_t opcode;
} ParsedInstruction;

void parse_noop(char *line, ParsedInstruction *inst);

void parse_inputc(char *line, ParsedInstruction *inst);

void parse_inputcf(char *line, ParsedInstruction *inst);

void parse_inputd(char *line, ParsedInstruction *inst);

void parse_inputdf(char *line, ParsedInstruction *inst);

void parse_move(char *line, ParsedInstruction *inst);

void parse_loadi(char *line, ParsedInstruction *inst);

void parse_loadp(char *line, ParsedInstruction *inst);

void parse_add(char *line, ParsedInstruction *inst);

void parse_addi(char *line, ParsedInstruction *inst);

void parse_sub(char *line, ParsedInstruction *inst);

void parse_subi(char *line, ParsedInstruction *inst);

void parse_load(char *line, ParsedInstruction *inst);

void parse_loadf(char *line, ParsedInstruction *inst);

void parse_store(char *line, ParsedInstruction *inst);

void parse_storef(char *line, ParsedInstruction *inst);

void parse_shiftl(char *line, ParsedInstruction *inst);

void parse_shiftr(char *line, ParsedInstruction *inst);

void parse_cmp(char *line, ParsedInstruction *inst);

void parse_jump(char *line, ParsedInstruction *inst);

void parse_bre(char *line, ParsedInstruction *inst);

void parse_brz(char *line, ParsedInstruction *inst);

void parse_brne(char *line, ParsedInstruction *inst);

void parse_brnz(char *line, ParsedInstruction *inst);

void parse_brg(char *line, ParsedInstruction *inst);

void parse_brge(char *line, ParsedInstruction *inst);

#endif