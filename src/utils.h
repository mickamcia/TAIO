#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "matrix.h"

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

void utils_print_execution_time(clock_t exact, clock_t approx);
void array_print(int* g, int size, char name[]);
void array2d_print(int* arr, int columns, int rows, char name[]);
void list_print(int* arr, int size);

void set_green_output_color();
void set_yellow_output_color();
void set_red_output_color();
void reset_output_color();
void print_test_pass(const char* test_name);
void print_test_approx_fail(const char* test_name);
void print_test_fail(const char* test_name);
void print_tests_summary(int passed, int failed, int approx_failed, clock_t elapsed);

void read_args(int argc, char** argv, bool* run_distance, bool* run_clique, bool* run_subgraph, bool* run_exact, bool* run_approx, bool* generate_stats, matrix** g1, matrix** g2);

double minimum(const double a, const double b);
double maximum(const double a, const double b);

#endif