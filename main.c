#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "vector.h"

const char *segments[] = {".data", ".code"};

#define NUM_SEGMENTS 2

const char *instructions[] = {"NOOP", "INPUTC", "INPUTCF", "INPUTD", "INPUTDF", "MOVE", "LOADI", "LOADP", "ADD", "ADDI", "SUB", "SUBI",
                                "LOAD", "LOADF", "STORE", "STOREF", "SHIFTL", "SHIFTR", "CMP", "JUMP", "BRE", "BRZ", "BRNE", "BRNZ",
                                "BRG", "BRGE"};

#define NUM_INSTRUCTIONS 26

// vector that will store the data segment
IntVector dseg;

// vector that will store the opcodes (code segment)
IntVector cseg;

void parse_dseg(char **lines, int offset, int len) {
    offset++; // skip the segment declaration
    for(int i = offset; i < len; i++) {
        // check if we reached a code segment
        if(strncmp(lines[i], segments[1], strlen(segments[1])) == 0) return;

        // variables to store name and type of data
        char name[32];
        char type[32];
        char value[32];

        int fields_parsed = sscanf(lines[i], " %s %s %s", name, type, value);

        if(fields_parsed == 3) printf("Name: %s\nType: %s\nValue: %s\n", name, type, value);
    }
}

void parse_cseg(char **lines, int offset, int len) {
    offset++; // skip the segment declaration
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

    // look for a valid segment to begin parsing
    for(int i = 0; i < num_lines; i++) {
        if(strncmp(lines[i], segments[0], strlen(segments[0])) == 0) parse_dseg(lines, i, num_lines);
        else if(strncmp(lines[i], segments[1], strlen(segments[1])) == 0) parse_cseg(lines, i, num_lines);
    }

    return 0;
}