#include "assembler.h"

int main (const int argc, const char* argv[])
{
    enum Asm_error error = ASM_NO_ERROR;
    size_t line_counter = 0;
    const char *NAME = argv[1];

    char** lines = process_file (NAME, &error, &line_counter);

    error = assembly_file (lines, line_counter);
    asm_print_error (error);

    return 0;
}
