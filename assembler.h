#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys\stat.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

enum Asm_error
{
    ASM_NO_ERROR = 0,
    ASM_NULL_PTR_FILE = 1,
    ASM_ERROR_FOPEN = 2,
    ASM_NULL_PTR_LOG = 3,
    ASM_ERROR_SSCANF = 4,
    ASM_ERROR_FSEEK = 5,
    ASM_ERROR_FTELL = 6,
    ASM_ERROR_STAT = 7,
    ASM_ERROR_STRCHR = 8,
    ASM_CALLOC_FAIL = 9,
    ASM_NOT_CMD = 10
};


enum Cmd
{
    PUSH = 1,
    ADD = 2,
    SUB = 3,
    MUL = 4,
    DIV = 5,
    OUT = 6,
    IN = 7,
    POP = 8,
    HLT = -1
};

enum Reg
{
    RAX = 1,
    RBX = 2,
    RCX = 3,
    RDX = 4
};

struct Regs
{
    const char* name;
    enum Reg number;
};

const Regs REG[] =
{
    {"RAX", RAX},
    {"RBX", RBX},
    {"RCX", RCX},
    {"RDX", RDX}
};

struct Cmds
{
    const char* name;
    enum Cmd number;
    bool has_arg;
    size_t length;
};

const Cmds CMD[] =
{
    {"PUSH", PUSH, true, 4},
    {"ADD", ADD, false, 3},
    {"SUB", SUB, false, 3},
    {"MUL", MUL, false, 3},
    {"DIV", DIV, false, 3},
    {"OUT", OUT, false, 3},
    {"IN", IN, false, 2},
    {"POP", POP, true, 3},
    {"HLT", HLT, false, 3}
};

enum Mask_arg
{
    MASK_RAM = 1 << 7,
    MASK_REG = 1 << 6,
    MASK_NUMBER = 1 << 5
};


enum Asm_error   my_fread                (size_t size, FILE *fp, char** buffer_ptr);
enum Asm_error   open_file_and_fill_stat (const char *NAME, struct stat *statbuf, FILE** file);
enum Asm_error   ptr_to_lines            (size_t size, size_t line_counter, char* buffer, char*** lines);
size_t           line_processing         (size_t size, char* buffer);
char**           process_file            (const char* NAME, enum Asm_error* error, size_t* line_counter);
enum Asm_error   asm_set_log_file        (FILE* file);
void             asm_print_error         (enum Asm_error error);
const char*      asm_get_error           (enum Asm_error error);
enum Asm_error   assembly_file           (char** lines, size_t line_counter);
void             skip_space              (char** line);

#endif // ASSEMBLER_H
