#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/**
 * This file contains function declarations for each of the assembly instructions so that they can be parsed properly. 
 */

typedef struct {
    char *inst;
    uint16_t opcode;
} ParsedInstruction;

// convenience function to add the name as a string to a ParsedInstruction struct
void add_inst_name(ParsedInstruction *inst, char *name);

#define MIN_REG 'A'
#define MAX_REG 'D'

// convenience function to check if a character specifies a valid CPU register
bool check_regs(char reg);

bool parse_noop(char *line, int line_num, ParsedInstruction *inst);

bool parse_inputc(char *line, int line_num, ParsedInstruction *inst);

bool parse_inputcf(char *line, int line_num, ParsedInstruction *inst);

bool parse_inputd(char *line, int line_num, ParsedInstruction *inst);

bool parse_inputdf(char *line, int line_num, ParsedInstruction *inst);

bool parse_move(char *line, int line_num, ParsedInstruction *inst);

bool parse_loadi(char *line, int line_num, ParsedInstruction *inst);

bool parse_loadp(char *line, int line_num, ParsedInstruction *inst);

bool parse_add(char *line, int line_num, ParsedInstruction *inst);

bool parse_addi(char *line, int line_num, ParsedInstruction *inst);

bool parse_sub(char *line, int line_num, ParsedInstruction *inst);

bool parse_subi(char *line, int line_num, ParsedInstruction *inst);

bool parse_load(char *line, int line_num, ParsedInstruction *inst);

bool parse_loadf(char *line, int line_num, ParsedInstruction *inst);

bool parse_store(char *line, int line_num, ParsedInstruction *inst);

bool parse_storef(char *line, int line_num, ParsedInstruction *inst);

bool parse_shiftl(char *line, int line_num, ParsedInstruction *inst);

bool parse_shiftr(char *line, int line_num, ParsedInstruction *inst);

bool parse_cmp(char *line, int line_num, ParsedInstruction *inst);

bool parse_jump(char *line, int line_num, ParsedInstruction *inst);

bool parse_bre(char *line, int line_num, ParsedInstruction *inst);

bool parse_brz(char *line, int line_num, ParsedInstruction *inst);

bool parse_brne(char *line, int line_num, ParsedInstruction *inst);

bool parse_brnz(char *line, int line_num, ParsedInstruction *inst);

bool parse_brg(char *line, int line_num, ParsedInstruction *inst);

bool parse_brge(char *line, int line_num, ParsedInstruction *inst);

#endif