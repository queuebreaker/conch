#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "conch.h"

#define MAX_LINES 20
#define VERSION 0.3

int main(void)
{
    char* lines[MAX_LINES];

    while (1)
    {
        CONCH_clean();
        int line_index = -1;
        char readline_prompt[32] = "\x1b[34mconch0.3>\x1b[0m ";

        while(1)
        {
            line_index++;
            lines[line_index] = readline(readline_prompt);
            add_history(lines[line_index]);

            if (lines[line_index] != NULL && strlen(lines[line_index]) > 0
                && (lines[line_index][strlen(lines[line_index]) - 1] == '\\'
                || lines[line_index][strlen(lines[line_index]) - 1] == '{'))
            {
                snprintf(readline_prompt, strlen(readline_prompt), "\x1b[34mline %d>\x1b[0m ", line_index + 2);
                lines[line_index][strlen(lines[line_index]) - 1] = '\0';
                continue;
            }
            break;
        }

        if (CONCH_create_src(lines, line_index) != 0)
        {
            printf("Source creation error; clean & continue...\n");
            continue;
        } else if (CONCH_compile() != 0)
        {
            printf("Compilation error; clean & continue...\n");
            continue;
        } else if (CONCH_exec() != 0)
        {
            printf("Execution error; clean & continue...\n");
            continue;
        };
        free(lines[line_index]);
        memset(lines, 0, sizeof(lines));
    }
    CONCH_clean();
    return 0;
}  