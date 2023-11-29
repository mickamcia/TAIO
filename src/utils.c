#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "limits.h"
#include "utils.h"
#include "graph.h"

void utils_print_execution_time(clock_t exact, clock_t approx) {
	printf("\nExact: %f seconds\n", ((double)exact) / CLOCKS_PER_SEC);
	printf("Approx: %f seconds\n", ((double)approx) / CLOCKS_PER_SEC);
}

void array_print(int* g, int size, char name[])
{
	printf("\n%s\n", name);
	for (int i = 0; i < size; i++) {
		const int val = g[i];
		if (val == INT_MAX) printf("inf ");
		else if (val >= -1) printf("% 3d ", val); // prints from 0 to 99, should change to "%d" by the end
		else printf("    ");
	}
	printf("\n");
}

void array2d_print(int* arr, int columns, int rows, char name[])
{
	printf("\n%s\n", name);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			const int val = arr[i * columns + j];
			if (val == INT_MAX) printf("inf ");
			else if (val >= -1) printf("% 3d ", val); // prints from 0 to 99, should change to "%d" by the end
			else printf("    ");
		}
		printf("\n");
	}
}

void list_print(int* arr, int size)
{
	printf("\nlist:\n");
	for (int i = 0; i < size; i++) {
		printf("% 3d ", arr[i]); 
		}
	printf("\n");
}


void set_green_output_color(){
	printf("\033[0;32m");
}

void set_yellow_output_color() {
	printf("\033[0;33m");
}

void set_red_output_color(){
	printf("\033[0;31m");
}

void reset_output_color(){
	printf("\033[0m");
}

void print_test_pass(const char* test_name){
	set_green_output_color();
	printf("\n%s PASSED\n", test_name);
	reset_output_color();
}

void print_test_approx_fail(const char* test_name) {
	set_yellow_output_color();
	printf("\n%s APPROX FAILED\n", test_name);
	reset_output_color();
}

void print_test_fail(const char* test_name){
	set_red_output_color();
	printf("\n%s FAILED\n", test_name);
	reset_output_color();
}

void print_tests_summary(int passed, int failed, int approx_failed, clock_t elapsed) {
	printf("\n-------------\n");
	printf("TESTS SUMMARY\n");
	printf("-------------\n");
	printf("Total execution time: %f seconds\n", ((double)elapsed) / CLOCKS_PER_SEC);
	printf("-------------\n");
	printf("\033[0;32m%s: %d\n", "PASSED", passed);
	printf("\033[0;33m%s: %d\n", "APPROX FAILED", approx_failed);
	printf("\033[0;31m%s: %d\n", "FAILED", failed);
	printf("\033[0m");
	printf("-------------\n");
}

void usage(char* programName) {
	fprintf(stderr, "\nUsage: %s [OPTIONS] [FILE_1] [FILE_2]\n", programName);
	fprintf(stderr, "If no option or file is provided, run the prepared tests.\n");
	fprintf(stderr, "If no option is given but FILE_1 is provided, find maximum clique.\n");
	fprintf(stderr, "If no option is given but FILE_1 and FILE_2 are provided, run all operations.\n");

	fprintf(stderr, "\nOptions:\n");
	fprintf(stderr, "\t-c\tFind maximum clique in graph from file FILE_1.\n");
	fprintf(stderr, "\t-d\tCalculate distance between graphs from files FILE_1 and FILE_2.\n");
	fprintf(stderr, "\t-s\tFind maximum common subgraph of graphs from files FILE_1 and FILE_2.\n");
	fprintf(stderr, "\t-g\tGenerate stats (time comparison of exact and approx algorithm).\n");
	fprintf(stderr, "\t-h\tDisplay help.\n\n");
	exit(EXIT_FAILURE);
}

void read_args(
	int argc, 
	char** argv, 
	bool* run_distance, 
	bool* run_clique, 
	bool* run_subgraph, 
	bool* generate_stats,
	matrix** g1, 
	matrix** g2) 
{
	bool d = false;
	bool c = false;
	bool s = false;
	bool g = false;
	*g1 = NULL;
	*g2 = NULL;

	int i = 1;
	for (; i < argc && argv[i][0] == '-'; i++) {
		switch (argv[i][1]) {
		case 'd': d = true; break;
		case 'c': c = true; break;
		case 's': s = true; break;
		case 'g': *generate_stats = true; return;
		case 'h': usage(argv[0]);
		default: usage(argv[0]);
		}
	}

	if (i < argc)
		*g1 = graph_load_from_file(argv[i]);

	if (++i < argc)
		*g2 = graph_load_from_file(argv[i]);
	
	if (((d || s) && *g2 == NULL) ||		// FILE_2 missing
		((c || d || s) && *g1 == NULL))		// FILE_1 missing
	{
		usage(argv[0]);
	}

	if (!c && !d && !s) {
		c = *g1 != NULL;
		d = s = *g2 != NULL;
	}

	*run_distance = d;
	*run_clique = c;
	*run_subgraph = s;
}

double minimum(const double a, const double b) {
	return a > b ? b : a;
}

double maximum(const double a, const double b) {
	return a > b ? a : b;
}