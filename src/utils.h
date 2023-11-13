#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

#endif