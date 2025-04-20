/* conch - a simple C REPL for GNU/Linux */
/* this software is licensed under GPLv3; for more details see LICENSE.md */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "config.h"

int
CONCH_create_src(char** lines, int line_index)
{
    FILE* file = fopen("/tmp/conchsrc.c", "w");
        
    for (long unsigned int i = 0; i < sizeof(libraries) / sizeof(libraries[0]); i++)
    {
        fprintf(file, "#include %s\n", libraries[i]);
    }

    fprintf(file, "int main() {");

    for (int i = 0; i <= line_index; i++)
    {
        if (lines[i] != NULL)
        {
            fprintf(file, "%s\n", lines[i]);
        }
    }

    fprintf(file, "}");
    fclose(file);

    return 0;
}

int
CONCH_compile(void)
{
    int stat;
    int cpid = fork();

    if (cpid == 0)
    {
        char* args[] = {"/usr/bin/gcc", "-o", "/tmp/conchbin", "/tmp/conchsrc.c", NULL};
        execv(args[0], args);
    }
    else if (cpid < 0)
    {
        return 1;
    }

    waitpid(cpid, &stat, 0);
    if (WIFEXITED(stat))
    {
        return WEXITSTATUS(stat);
    }

    return 1;
}

int
CONCH_exec(void)
{
    int stat;
    int cpid = fork();

    if (cpid == 0)
    {
        char* args[] = {"/tmp/conchbin", NULL};
        execv(args[0], args);
    }
    else if (cpid < 0)
    {
        return 1;
    }

    waitpid(cpid, &stat, 0);
    if (WIFEXITED(stat))
    {
        return WEXITSTATUS(stat);
    }

    return 1;
}

int
CONCH_clean(void)
{
    if (remove("/tmp/conchsrc.c") == 0)
    {
        if (remove("/tmp/conchbin") == 0)
        {
            return 0;
        }
    }
    
    return 1;
}

int
main(void)
{
    char* lines[max_lines];
    char* readline_prompt;
    char* command;
    int line_index;

    while (1)
    {
        CONCH_clean();

        readline_prompt = (char*)default_prompt;
        line_index = -1;

        while(1)
        {
            line_index++;
            command = readline(readline_prompt);
            add_history(command);

            lines[line_index] = command;

            if (command != NULL && strlen(command) > 0
                && (command[strlen(command) - 1] == '\\'
                || command[strlen(command) - 1] == '{'))
            {
                asprintf(&readline_prompt, "\x1b[34m(%d)>\x1b[0m ", line_index + 2);
                lines[line_index][strlen(lines[line_index]) - 1] = '\0';
                continue;
            }
            break;
        }

        if (CONCH_create_src(lines, line_index) != 0)
        {
            printf("Source creation error; clean & continue...\n");
            continue;
        }
        else if (CONCH_compile() != 0)
        {
            printf("Compilation error; clean & continue...\n");
            continue;
        }
        else if (CONCH_exec() != 0)
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