#include "assembler.h"

int main (const int argc, const char* argv[])  // добавить проверку argc
{
    enum Asm_error error = ASM_NO_ERROR;
    size_t line_counter = 0;
    Mark* marks = NULL;
    int n_marks = 0;
    char* names_array = NULL;
    const char *NAME = argv[1];

    char** lines = process_file (NAME, &error, &line_counter);
    allocate_mem (&marks, &names_array);

    error = assembly_file (lines, line_counter, marks, &n_marks);
    asm_print_error (error);

    error = assembly_file (lines, line_counter, marks, &n_marks);
    asm_print_error (error);

    free (marks);
    free (names_array);
    free (*lines);
    free (lines);
    return 0;
}
