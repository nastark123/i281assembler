#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "instructions.h"
#include "vector.h"

#define DSEG_SIZE 16
#define CSEG_SIZE 64

const char *segments[] = {".data", ".code"};

#define NUM_SEGMENTS 2

const char *instructions[] = {"NOOP", "INPUTC", "INPUTCF", "INPUTD", "INPUTDF", "MOVE", "LOADI", "LOADP", "ADD", "ADDI", "SUB", "SUBI",
                                "LOAD", "LOADF", "STORE", "STOREF", "SHIFTL", "SHIFTR", "CMP", "JUMP", "BRE", "BRZ", "BRNE", "BRNZ",
                                "BRG", "BRGE"};

#define NOOP 0x00
#define INPUTC 0x01
#define INPUTCF 0x02
#define INPUTD 0x03
#define INPUTDF 0x04
#define MOVE 0x05
#define LOADI 0x06
#define LOADP 0x07
#define ADD 0x08
#define ADDI 0x09
#define SUB 0x0A
#define SUBI 0x0B
#define LOAD 0x0C
#define LOADF 0x0D
#define STORE 0x0E
#define STOREF 0x0F
#define SHIFTL 0x10
#define SHIFTR 0x11
#define CMP 0x12
#define JUMP 0x13
#define BRE 0x14
#define BRZ 0x15
#define BRNE 0x16
#define BRNZ 0x17
#define BRG 0x18
#define BRGE 0x19

#define NUM_INSTRUCTIONS 26

typedef struct {
    uint8_t len;
    uint8_t *data;
    char *name;
    uint8_t start_address;
} DataLabel;

typedef struct {
    char *name;
    uint8_t address;
} BranchDest;

// vector that will store the data segment
IntVector dseg;

// vector that will store the opcodes (code segment)
IntVector cseg;

int parse_dseg(char **lines, int offset, int lines_len, DataLabel *labels, int labels_len) {
    offset++; // skip the segment declaration

    int labels_index = 0; // index in the labels array
    uint8_t dseg_address = 0; // the current address in the data segment, this always counts up so that the assembler knows where labels are
    for(int i = offset; i < lines_len; i++) {
        // check if we reached a code segment
        if(strncmp(lines[i], segments[1], strlen(segments[1])) == 0) return labels_index;

        // variables to store name and type of data
        char name[32];
        char type[32];
        char value[32];

        int fields_parsed = sscanf(lines[i], " %s %s %s", name, type, value);

        // get the whole string for the value
        char *val_start = strstr(lines[i], value);

        if(fields_parsed == 3) {
            DataLabel label;
            label.data = calloc(16, sizeof(uint8_t));
            label.len = 16;
            label.name = malloc(sizeof(char) * (strlen(name) + 1));
            strcpy(label.name, name);

            int tokens_parsed = 0;

            char *tok = strtok(val_start, ",");
            while(tok != NULL) {
                sscanf(tok, " %hhu ", &(label.data[tokens_parsed++]));
                tok = strtok(NULL, ",");
            }

            label.data = realloc(label.data, sizeof(uint8_t) * tokens_parsed);
            label.len = tokens_parsed;
            label.start_address = dseg_address;
            
            dseg_address += tokens_parsed;
            // check if we have exceeded the maximum number of bytes that can be stored in the data segment
            if(dseg_address >= DSEG_SIZE) {
                printf("Error during assembly, too many bytes in data segment\n");
                return -1;
            }

            labels[labels_index++] = label;
        }


    }

    return labels_index;
}

// converts a string instruction to an integer id
int inst_to_id(char *inst) {
    for(int i = 0; i < NUM_INSTRUCTIONS; i++) {
        if(strcmp(inst, instructions[i]) == 0) return i;
    }

    return -1;
}

void parse_cseg(char **lines, int offset, int lines_len, ParsedInstruction *instructions, int inst_len) {
    offset++; // skip the segment declaration
    for(int i = offset; i < lines_len; i++) {
        char inst_str[32];
        sscanf(lines[i], " %s ", inst_str);
        int id = inst_to_id(inst_str);
        if(id < 0) {
            printf("Invalid instruction found at line %d\n", i + 1); // add 1 to i since we start line indexing at 0, whereas the text editor starts at 1
            exit(-1);
        }

        ParsedInstruction inst;

        bool success = false;

        switch(id) {
            case NOOP:
                parse_noop(lines[i], i + 1, &inst);
                break;

            case INPUTC:
                parse_inputc(lines[i], i + 1, &inst);
                break;

            case INPUTCF:
                parse_inputcf(lines[i], i + 1, &inst);
                break;


            case INPUTD:
                parse_inputd(lines[i], i + 1, &inst);
                break;


            case INPUTDF:
                parse_inputdf(lines[i], i + 1, &inst);
                break;


            case MOVE:
                parse_move(lines[i], i + 1, &inst);
                break;


            case LOADI:
                parse_loadi(lines[i], i + 1, &inst);
                break;

            case LOADP:
                parse_loadp(lines[i], i + 1, &inst);
                break;

            case ADD:
                parse_add(lines[i], i + 1, &inst);
                break;

            case ADDI:
                parse_addi(lines[i], i + 1, &inst);
                break;

            case SUB:
                parse_sub(lines[i], i + 1, &inst);
                break;

            case SUBI:
                parse_subi(lines[i], i + 1, &inst);
                break;

            case LOAD:
                parse_load(lines[i], i + 1, &inst);
                break;

            case LOADF:
                parse_loadf(lines[i], i + 1, &inst);
                break;

            case STORE:
                parse_store(lines[i], i + 1, &inst);
                break;

            case STOREF:
                parse_storef(lines[i], i + 1, &inst);
                break;

            case SHIFTL:
                parse_shiftl(lines[i], i + 1, &inst);
                break;

            case SHIFTR:
                parse_shiftr(lines[i], i + 1, &inst);
                break;

            case CMP:
                parse_cmp(lines[i], i + 1, &inst);
                break;

            case JUMP:
                parse_jump(lines[i], i + 1, &inst);
                break;

            case BRE:
                parse_bre(lines[i], i + 1, &inst);
                break;

            case BRZ:
                parse_brz(lines[i], i + 1, &inst);
                break;

            case BRNE:
                parse_brne(lines[i], i + 1, &inst);
                break;

            case BRNZ:
                parse_brnz(lines[i], i + 1, &inst);
                break;

            case BRG:
                parse_brg(lines[i], i + 1, &inst);
                break;

            case BRGE:
                parse_brge(lines[i], i + 1, &inst);
                break;

            default: // not sure how we'd ever get to here, but just in case
                printf("Error on line %d, unrecognized instruction\n", i + 1);
                exit(-1);

        }
    }
}

void replace_dseg_labels(char **lines, int offset, int lines_len, DataLabel *labels, int labels_len) {
    offset++; // skip the segment declaration for the code segment

    // replace all data symbol names with their address
    for(int i = offset; i < lines_len; i++) {
        for(int j = 0; j < labels_len; j++) {
            char *c = strstr(lines[i], labels[j].name);
            if(c != NULL) {
                // this seems sketchy, but oh well
                char *edited_line = malloc(sizeof(char) * (strlen(lines[i]) + 3)); // + 3 is here since the maximum number of chars an address can take is 3, and the minimum for a label is 1, also need the null terminator

                // copy the contents of the line before the symbol
                strncpy(edited_line, lines[i], (c - lines[i]) / sizeof(char));
                edited_line[(c - lines[i]) / sizeof(char)] = '\0';

                // concatenate the address
                sprintf(edited_line, "%s%d%s", edited_line, labels[j].start_address, c + (strlen(labels[j].name) * sizeof(char)));

                free(lines[i]);
                lines[i] = edited_line;

                // printf("Line %d: %s\n", i, edited_line);
            }
        }
    }
}

int parse_branch_dest(char **lines, int offset, int lines_len, BranchDest *dest, int dest_len) {
    offset++; // skip the code segment declaration

    int dest_index = 0; // current destination index
    for(int i = offset; i < lines_len; i++) {
        char *c = strchr(lines[i], ':'); // if a line contains a colon, it has a branch label

        if(c != NULL) {

            size_t label_len = (c - lines[i]) / sizeof(char);

            dest[dest_index].name = malloc(sizeof(char) * (label_len + 1));
            strncpy(dest[dest_index].name, lines[i], label_len);
            dest[dest_index].name[label_len] = '\0';

            dest[dest_index].address = i - offset;

            // edit the line to remove the label, leaving only the assembly instruction
            strcpy(lines[i], c + 1); // add 1 to skip the colon
            lines[i] = realloc(lines[i], sizeof(char) * (strlen(lines[i]) + 1));

            dest_index++;
        }
    }

    // replace all label names with their address
    for(int i = offset; i < lines_len; i++) {
        for(int j = 0; j < dest_index; j++) {
            char *c = strstr(lines[i], dest[j].name);
            if(c != NULL) {
                // this seems sketchy, but oh well
                char *edited_line = malloc(sizeof(char) * (strlen(lines[i]) + 3)); // + 3 is here since the maximum number of chars an address can take is 3, and the minimum for a label is 1, also need the null terminator

                // copy the contents of the line before the symbol
                strncpy(edited_line, lines[i], (c - lines[i]) / sizeof(char));
                edited_line[(c - lines[i]) / sizeof(char)] = '\0';

                // concatenate the address
                // the value passed to the %d specifier is the destination address minus the current program counter value - 1
                sprintf(edited_line, "%s%d%s", edited_line, dest[j].address - i + offset - 1, c + (strlen(dest[j].name) * sizeof(char)));

                free(lines[i]);
                lines[i] = edited_line;

                // printf("Line %d: %s\n", i, edited_line);
            }
        }
    }

    return dest_index;
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("Usage: filename\n");
        return -1;
    }

    FILE *asm_file = fopen(argv[1], "r");
    if(asm_file == NULL) {
        printf("Error occured opening file: %s\n", strerror(errno));
        return -1;
    }

    // initialize the vectors
    create_new(&dseg, NULL, 16);
    create_new(&cseg, NULL, 64);

    // find the size of the file
    fseek(asm_file, 0, SEEK_END);
    long length = ftell(asm_file);
    fseek(asm_file, 0, SEEK_SET);

    // find number of lines in program so that we can parse line by line
    int num_lines = 0;
    char c;
    while((c = fgetc(asm_file)) != EOF) {
        if(c == '\n') num_lines++;
    }

    // allocate a two-dimensional array to store each line
    char **lines = malloc(sizeof(char *) * num_lines);

    // buffer to read data into
    char *read_buff = malloc(sizeof(char) * 1024);
    memset(read_buff, 0, 1024);

    fseek(asm_file, 0, SEEK_SET);

    for(int i = 0; i < num_lines; i++) {
        fgets(read_buff, 1024, asm_file);
        size_t len = strlen(read_buff);
        read_buff[len - 1] = '\0'; // remove trailing newline
        lines[i] = malloc(sizeof(char) * (len + 1));
        if(lines[i] == NULL) printf("Error allocating memory\n");
        strcpy(lines[i], read_buff);
        memset(read_buff, 0, 1024);
    }

    // remove comments from file
    for(int i = 0; i < num_lines; i++) {
        char *comment_start = strchr(lines[i], ';');
        if(comment_start != NULL) {
            comment_start[0] = '\0';
        }
        lines[i] = realloc(lines[i], strlen(lines[i]) + 1);
    }

    // data label array to store values that will be placed in the data segment
    DataLabel *label = malloc(sizeof(DataLabel) * 8);
    int num_labels = 0;

    // look for a data segment to begin parsing
    for(int i = 0; i < num_lines; i++) {
        if(strncmp(lines[i], segments[0], strlen(segments[0])) == 0) {
            num_labels = parse_dseg(lines, i, num_lines, label, 8);

            printf("Read %d labels from data segment\n", num_labels);

            // for(int j = 0; j < num_labels; j++) {
            //     printf("Name: %s\nStart Address: %d\nValues:\n", label[j].name, label[j].start_address);
            //     for(int k = 0; k < label[j].len; k++) {
            //         printf("%d\n", label[j].data[k]);
            //     }
            // }
        }
    }

    // array to store parsed instructions
    ParsedInstruction *inst = malloc(sizeof(ParsedInstruction) * 64);
    int num_insts = 0;

    // array to store branch destinations
    BranchDest *dest = malloc(sizeof(BranchDest) * 16);
    int num_branches = 0;

    // look for a code segment to parse code
    for(int i = 0; i < num_lines; i++) {
        if(strncmp(lines[i], segments[1], strlen(segments[1])) == 0) {
            replace_dseg_labels(lines, i, num_lines, label, num_labels);
            num_branches = parse_branch_dest(lines, i, num_lines, dest, 16);

            for(int j = 0; j < num_lines; j++) {
                printf("%d: %s\n", j + 1, lines[j]);
            }
            parse_cseg(lines, i, num_lines, inst, 64);
        }
    }

    // printf("Found %d branches\n", num_branches);
    // for(int i = 0; i < num_branches; i++) {
    //     printf("Label: %s\nPC: %d\n", dest[i].name, dest[i].address);
    // }

    return 0;
}