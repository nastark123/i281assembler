#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "vector.h"

#define DSEG_SIZE 16
#define CSEG_SIZE 64

const char *segments[] = {".data", ".code"};

#define NUM_SEGMENTS 2

const char *instructions[] = {"NOOP", "INPUTC", "INPUTCF", "INPUTD", "INPUTDF", "MOVE", "LOADI", "LOADP", "ADD", "ADDI", "SUB", "SUBI",
                                "LOAD", "LOADF", "STORE", "STOREF", "SHIFTL", "SHIFTR", "CMP", "JUMP", "BRE", "BRZ", "BRNE", "BRNZ",
                                "BRG", "BRGE"};

#define NUM_INSTRUCTIONS 26

typedef struct {
    uint8_t len;
    uint8_t *data;
    char *name;
    uint8_t start_address;
} DataLabel;

typedef struct {
    char *inst;
    uint16_t opcode;
} ParsedInstruction;

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

// TODO clean up this function, it seems too lengthy
void parse_cseg(char **lines, int offset, int lines_len, ParsedInstruction *instructions, int inst_len, DataLabel *labels, int labels_len) {
    offset++; // skip the segment declaration

    // replace all data symbol names with their address
    // TODO move this to its own function
    for(int i = offset; i < lines_len; i++) {
        for(int j = 0; j < labels_len; j++) {
            char *c = strstr(lines[i], labels[j].name);
            if(c != NULL) {
                // this seems sketchy, but oh well
                char *edited_line = malloc(sizeof(char) * (strlen(lines[i]) + 3)); // + 3 is here since the maximum number of chars an address can take is 3, and the minimum for a label is 1, also need the null terminator
                
                // copy the contents of the line before the symbol
                strncpy(edited_line, lines[i], (c - lines[i]) / sizeof(char));
                edited_line[(c - lines[i]) / sizeof(char)] = '\0';

                // // concatenate the address
                sprintf(edited_line, "%s%d%s", edited_line, labels[j].start_address, c + (strlen(labels[j].name) * sizeof(char)));

                printf("Line %d: %s\n", i, edited_line);
            }
        }
    }
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

    ParsedInstruction *inst = malloc(sizeof(ParsedInstruction) * 64);

    // look for a code segment to parse code
    for(int i = 0; i < num_lines; i++) {
        if(strncmp(lines[i], segments[1], strlen(segments[1])) == 0) {
            parse_cseg(lines, i, num_lines, inst, 64, label, num_labels);
        }
    }

    return 0;
}