#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    for (;;)
    {   
        char command[255];
        printf("conch0.1> ");
        fgets(command, 255, stdin);

        FILE* file = fopen("/tmp/conchsrc.c", "w");
        
        fprintf(file, "#include <stdio.h>\n#include <unistd.h>\n\n");
        fprintf(file, "int main() {\n");

        fprintf(file, "%s ", command);

        fprintf(file, "\n}\n");
        fclose(file);

        int cpid = fork();

        if (cpid == 0)
        {
            char *gcc_argv[] = {"/usr/bin/gcc", "-o", "/tmp/conchbin", "/tmp/conchsrc.c", NULL};
            execv(gcc_argv[0], gcc_argv);
        }
        else if (cpid > 0)
        {
            int pid = waitpid(cpid, NULL, 0);

            int cpid2 = fork();
            if (cpid2 == 0)
            {
                char *bin_argv[] = {"/tmp/conchbin", NULL};
                execv(bin_argv[0], bin_argv);
            }
            else if (cpid2 > 0)
            {
                waitpid(cpid2, NULL, 0);
            }
        }
        else
        {
            printf("fork failed\n");
        }
    }
    
}
