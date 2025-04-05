#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    char* bin_argv[] = {"/tmp/conchbin", NULL};
    execv(bin_argv[0], bin_argv);
}

