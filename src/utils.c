#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

void utils_print_execution_time(clock_t exact, clock_t approx) {
	printf("\nExact: %f seconds\n", ((double)exact) / CLOCKS_PER_SEC);
	printf("Approx: %f seconds\n", ((double)approx) / CLOCKS_PER_SEC);
}