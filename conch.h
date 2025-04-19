#ifndef CONCH_H
#define CONCH_H

int CONCH_create_src(char** lines, int num_lines);
int CONCH_compile(void);
int CONCH_exec(void);
int CONCH_clean(void);

#endif // conch.h