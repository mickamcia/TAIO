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

void tests_metric(int* passed, int* failed);
void tests_clique(int* passed, int* failed);
void tests_subgraph(int* passed, int* failed);

#endif