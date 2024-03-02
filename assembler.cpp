#include "assembler.h"

#define PRINT_BEGIN() fprintf (log_file, "begin: %s.\n", __PRETTY_FUNCTION__)

#define PRINT_END() fprintf (log_file, "success end: %s.\n", __PRETTY_FUNCTION__)

static FILE* log_file = stderr;

const int MAX_SYMB = 20;

const int N_REGS = 4;

const size_t N_CMDS = sizeof (CMD) / sizeof (CMD[0]);

enum Asm_error asm_set_log_file (FILE* file)
{
    PRINT_BEGIN();
    if (file == NULL)
        return ASM_NULL_PTR_LOG;
    log_file = file;
    PRINT_END();
    return ASM_NO_ERROR;
}

enum Asm_error my_fread (size_t size, FILE *fp, char** buffer_ptr)
{
    PRINT_BEGIN();
    if (fp == NULL)
        return ASM_NULL_PTR_FILE;

    char* buffer = (char*) calloc (size + 1, sizeof (char));
    if (buffer == NULL)
        return ASM_CALLOC_FAIL;

    *buffer_ptr = buffer;
    int position = ftell (fp);
    if (fseek (fp, 0, SEEK_SET) != 0)
        return ASM_ERROR_FSEEK;
    int temp = 0;
    for (size_t i = 0; i < size && (temp = fgetc (fp)) != EOF; i++)
    {
        buffer[i] = (char) temp;
        continue;
    }
    if (fseek (fp, position, SEEK_SET) != 0)
        return ASM_ERROR_FSEEK;

    PRINT_END();
    return ASM_NO_ERROR;
}

enum Asm_error open_file_and_fill_stat (const char *NAME, struct stat *statbuf, FILE** file)
{
    PRINT_BEGIN();
    *file = fopen (NAME, "rb");
    if (*file == NULL)
        return ASM_ERROR_FOPEN;
    if (stat (NAME, statbuf))
        return ASM_ERROR_STAT;
    PRINT_END();
    return ASM_NO_ERROR;
}

enum Asm_error ptr_to_lines (size_t size, size_t line_counter, char* buffer, char*** lines)
{
    PRINT_BEGIN();
    char** temp_lines = (char**) calloc (line_counter + 1, sizeof (char*));
    if (temp_lines == NULL)
    {
        return ASM_CALLOC_FAIL;
    }
    *lines = temp_lines;
    size_t a = 0;
    (*lines)[a++] = buffer;
    for (size_t k = 0; k < size + 1; k++)
    {
        if (buffer[k] == '\0')
        {
            k += 2;
            (*lines)[a] = buffer + k;
            a++;
        }
    }
    PRINT_END();
    return ASM_NO_ERROR;
}

size_t line_processing (size_t size, char* buffer)
{
    PRINT_BEGIN();
    size_t line_counter = 0;
    for (size_t k = 0 ; k < size; k++)
    {
        if (buffer[k] == '\r')
        {
            line_counter++;
            buffer[k] = '\0';
            k++;
            continue;
        }
        if (buffer[k] == '\n')
        {
            line_counter++;
            buffer[k] = '\0';
            continue;
        }
    }

    if (buffer[size - 1] != '\n')
        line_counter++;

    PRINT_END();
    return line_counter;
}

enum Asm_error assembly_file (char** lines, size_t line_counter) // FILE* asm_file, char** output_str
{
    PRINT_BEGIN();
    FILE *assemble_file = fopen ("assemble_file.txt", "wb"); // super cringe
    if (assemble_file == NULL)
        return ASM_ERROR_FOPEN;
    char cmd[MAX_SYMB] = {0};
    char* bin_cmd = (char*) calloc (line_counter, sizeof (double) + sizeof (char)); // структуру в реалокацию
    size_t bin_pos = 0;
    /////// error check? // I DONT UNDERSTAND ЗАПИСАННЫЕ БАЙТЫ
    double digit = NAN; // переменные в структуру

    printf("%d\n", line_counter);

    for (size_t n_line = 0; n_line < line_counter; n_line++) // line_number (n_line)
    {
        char* cur_line = lines[n_line];
        int len_cmd = 0;
        len_cmd = sscanf (cur_line, "%s", cmd);
        if (len_cmd == -1) // длину забрать
        {
            return ASM_ERROR_SSCANF;
        }
        bool is_cmd = false;
        for (size_t j = 0; j < N_CMDS; j++)
        {
            if (strncmp (cmd, CMD[j].name, len_cmd) != 0)
            {
                continue;
            }
            is_cmd = true;
            bin_cmd[bin_pos] = CMD[j].number;
            cur_line += CMD[j].length;
            if (CMD[j].has_arg)
            {
                bool has_reg = false; // skipspace!!!
                skip_space (&cur_line); // разбить на функции
                for (size_t i = 0; i < N_REGS; i++)
                {
                    if (strncmp (cur_line, REG[i].name, MAX_SYMB) == 0)
                    {
                        has_reg = true; // try ... register s proverkoy kotoriy vernet true esli nashel i false esli net
                        break;
                    }
                }
                if (has_reg)
                {
                    bin_cmd[bin_pos] |= MASK_REG;
                    bin_pos += sizeof (char);
                    bin_cmd[bin_pos] = *cur_line;
                    bin_pos += sizeof (char);
                }
                else
                {
                    bin_cmd[bin_pos] |= MASK_NUMBER;
                    bin_pos += sizeof (char);
                    sscanf (cur_line, "%lf", bin_cmd + bin_pos);
                    bin_pos += sizeof (double);
                    //*(double*)(bin_cmd + position) = *(double*)(cur_line); // scanf
                }
            }
            else
            {
                bin_pos += sizeof (char);
            }
        }
        if (!is_cmd)
        {
            return ASM_NOT_CMD;
        }
    }
    fwrite (bin_cmd, sizeof (char), bin_pos, assemble_file);
    fclose (assemble_file);
    PRINT_END();
    return ASM_NO_ERROR;
} // 1 - узнает, что за команда
//2 - try register
//2 - try number
//разбить на хорошие функции

void skip_space (char** line)
{
    for (size_t i = 0; isspace (**line); i++)
    {
        (*line)++;
    }
}

void asm_print_error (enum Asm_error error)
{
    PRINT_BEGIN();
    fprintf (log_file, "%s\n", asm_get_error (error));
    PRINT_END();
}

const char* asm_get_error (enum Asm_error error)
{
    PRINT_BEGIN();
    switch (error)
    {
        case ASM_NO_ERROR:
            return "Ass: Ошибок в работе функций не выявлено.";
        case ASM_NULL_PTR_FILE:
            return "Ass: Передан нулевой указатель на файл.";
        case ASM_ERROR_FOPEN:
            return "Ass: Ошибка в работе функции fopen.";
        case ASM_NULL_PTR_LOG:
            return "Ass: Вместо адреса log_file передан нулевой указатель.";
        case ASM_ERROR_SSCANF:
            return "Ass: Ошибка в работе функции sscanf.";
        case ASM_ERROR_FSEEK:
            return "Ass: Ошибка в работе функции fseek.";
        case ASM_ERROR_FTELL:
            return "Ass: Ошибка в работе функции ftell.";
        case ASM_ERROR_STAT:
            return "Ass: Ошибка в работе функции stat.";
        case ASM_ERROR_STRCHR:
            return "Ass: Ошибка в работе функции strchr.";
        case ASM_CALLOC_FAIL:
            return "Ass: Ошибка в выделении памяти (calloc).";
        case ASM_NOT_CMD:
            return "Asm: Не команда.";
        default:
            return "Ass: Куда делся мой enum ошибок?";
    }
}


char** process_file (const char* NAME, enum Asm_error* error, size_t* line_counter)
{
    struct stat statbuf = {};
    FILE *source_code_file = NULL;
    *error = open_file_and_fill_stat (NAME, &statbuf, &source_code_file);
    asm_print_error (*error);
    char *buffer = NULL;
    *error = my_fread (statbuf.st_size, source_code_file, &buffer);
    asm_print_error (*error);

    *line_counter = line_processing (statbuf.st_size, buffer);
    asm_print_error (*error);
    char** lines = NULL;
    *error = ptr_to_lines (statbuf.st_size, *line_counter, buffer, &lines);
    asm_print_error (*error);
    return lines;
}

