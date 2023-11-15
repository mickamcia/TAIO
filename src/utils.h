#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <time.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

void utils_print_execution_time(clock_t exact, clock_t approx);

#endif