#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>

int ret;
char* command;

int CONCH_create_src(char* body) {
        FILE* file = fopen("/tmp/conchsrc.c", "w");
        
        fprintf(file, "#include <stdio.h>\n#include <unistd.h>\n\n");
        fprintf(file, "int main() {\n");

        fprintf(file, "%s ", body);

        fprintf(file, "\n}\n");
        fclose(file);

        return 0;
}

int CONCH_compile(void) {
    int stat;
    int cpid = fork();

    if (cpid == 0) {
        char* args[] = {"/usr/bin/gcc", "-o", "/tmp/conchbin", "/tmp/conchsrc.c", NULL};
        execv(args[0], args);
    } else if (cpid < 0) {
        return 1;
    }

    waitpid(cpid, &stat, 0);
    if (WIFEXITED(stat)) {
        return WEXITSTATUS(stat);
    }

    return 1;
}

int CONCH_exec(void) {
    int stat;
    int cpid = fork();

    if (cpid == 0) {
        char* args[] = {"/tmp/conchbin", NULL};
        execv(args[0], args);
    } else if (cpid < 0) {
        return 1;
    }

    waitpid(cpid, &stat, 0);
    if (WIFEXITED(stat)) {
        return WEXITSTATUS(stat);
    }

    return 1;
}

int CONCH_clean(void) {
    free(command);
    if (remove("/tmp/conchsrc.c") == 0) {
        if (remove("/tmp/conchbin") == 0) {
            return 0;
        }
    }
    
    return 1;
}

int main(void) {

    using_history();

    while (1) {
        ret = 0;
        command = readline("\x1b[34mconch0.2>\x1b[0m ");

        add_history(command);

        if (!strcmp(command, "exit")) {
            exit(0);
        }

        ret = CONCH_create_src(command);
        if (ret != 0) {
            printf("Injection error; clean & continue...\n");
            CONCH_clean();
            continue;
        }

        ret = CONCH_compile();
        if (ret != 0) {
            printf("Compilation error; clean & continue...\n");
            CONCH_clean();
            continue;
        }

        ret = CONCH_exec();
        if (ret != 0) {
            printf("Execution error; clean & continue...\n");
            CONCH_clean();
            continue;
        }

        ret = CONCH_clean();
        if (ret != 0) {
            printf("Clean error; continue...\n");
            continue;
        }
    }

    clear_history();
}