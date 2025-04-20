/* max number of lines (applies to backslash-escaped input) */
static const unsigned int max_lines = 20;

/* default readline prompt */
static const char* default_prompt   = { "\x1b[34mconch0.4>\x1b[0m " };

/* libraries to be included in generated source files by default. note that system-wide headers have to be surrounded with angle brackets */
static const char* libraries[]      = { "<stdio.h>",
                                        "<unistd.h>"};