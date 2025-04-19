#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>

int CONCH_create_src(char** lines, int line_index) {
        FILE* file = fopen("/tmp/conchsrc.c", "w");
        
        fprintf(file, "#include <stdio.h>\n#include <unistd.h>\n\n");
        fprintf(file, "int main() {\n");

        for (int i = 0; i <= line_index; i++) {
            if (lines[i] != NULL) {
                fprintf(file, "%s\n", lines[i]);
            }
        }

        fprintf(file, "}\n");
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
    if (remove("/tmp/conchsrc.c") == 0) {
        if (remove("/tmp/conchbin") == 0) {
            return 0;
        }
    }
    
    return 1;
}