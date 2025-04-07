сonch (pronounced *KAHN-tch*) is a very simple C REPL for Linux.

**NOTE:** This project is in very early alpha, being developed for fun by an amateur and _absolutely_ not ready for any kind of "production".

## How it works

Having received an input (for example, ```printf("hello world");```), conch creates a file called ```conchsrc.c``` in the ```/tmp/``` directory with the following contents (function ```CONCH_create_src```):

```c
#include <stdio.h>
#include <unistd.h>

void main(void) {
    printf("hello world"); // <- the input
    return 0;
}
```

It then forks, with the child executing ```gcc -o /tmp/conchbin /tmp/conchsrc.c``` and exiting (function ```CONCH_compile```), after which it forks again, this time in order for the child to run the compiled binary (function ```CONCH_exec```). In finalizing steps, conch deletes the temporary files, frees the input pointer (function ```CONCH_clean```) and repeats the REPL loop.  

## Building and installing

To build, run ```make (clean all)```.  
To install, run ```sudo make (clean) install```.

## TODO (in rough order of urgency)

* draw a pretty logo;
* add input parsing;
* add better error handling;
* refactor everything;
* add simple configuration capabilities (via ```config.h``` or something along the lines);
* add procedural file inclusion;
* add EOL escaping;
* add stdout piping;
* [...]

## Licensing

This work is licensed under the GNU General Public License v3.0; see ```LICENSE.md``` for more details.
