**[SUPER WIP]** A very simple C interpreter for Linux.

## How it works

Upon receiving an input from the REPL (for example, ```printf("hello world");```), conch will create a file called ```conchsrc.c``` in the ```/tmp/``` directory with these contents (function ```CONCH_create_src```):

```c
#include <stdio.h>
#include <unistd.h>

void main(void) {
    printf("hello world");
    return 0;
}
```

It will then fork, with the child executing ```gcc -o /tmp/conchbin /tmp/conchsrc.c``` and exiting (function ```CONCH_compile```). It will then fork the second time, this time in order for the child to finally run the compiled binary (function ```CONCH_exec```). After that, conch will delete the temporary files and free the input pointer (function ```CONCH_clean```) and repeat the loop.  

## Building and installing

To build, run ```make (clean all)```.  
To install, run ```sudo make (clean) install```.

## TODO (in order of urgency)

* a pretty logo;
* input parsing;
* better error handling;
* refactor everything 100 times over;
* simple configuration capabilities (via ```config.h``` or something along the lines);
* procedural file inclusion;
* EOL escaping;
* stdout piping;

## Licensing

This work is licensed under CC BY 4.0. See ```LICENSE.md``` for more details.
