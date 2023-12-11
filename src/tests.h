#ifndef TESTS_H
#define TESTS_H

#include <stdio.h>
#include <time.h>
#include "graph.h"
#include "approx_clique.h"
#include "approx_subgraph.h"
#include "exact_clique.h"
#include "exact_subgraph.h"
#include "exact_clique_bb.h"
#include "bma.h"
#include "utils.h"
#include "metric.h"

#define PAUSE_AFTER_TEST 0
#define TEST_SAMPLING 10
#define GENERATE_EXAMPLES 0

#define PAUSE() (PAUSE_AFTER_TEST == 1 && fprintf(stdout, "Press any key to continue...\n") && getchar() )

void tests_metric(int* passed, int* failed);
void tests_clique(int* passed, int* failed, int* approx_failed);
void tests_subgraph(int* passed, int* failed, int* approx_failed);

void test_metric_from_args(matrix* g1, matrix* g2, int* passed, int* failed);
void test_clique_from_args(matrix* g, bool run_exact, bool run_approx, int* passed, int* failed, int* approx_failed);
void test_subgraph_from_args(matrix* g1, matrix* g2, bool run_exact, bool run_approx, int* passed, int* failed, int* approx_failed);

void test_clique_stats(int* passed, int* failed, int* approx_failed);
void test_subgraph_stats(int* passed, int* failed, int* approx_failed);

#endif