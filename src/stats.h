#ifndef STATS_H
#define STATS_H

#include <stdio.h>
#include <time.h>

typedef struct {
    double exact_time;
    double approx_time;
} stats;

stats* stats_create(clock_t exact_time, clock_t approx_time);

FILE* stats_clique_open_csv();
void stats_clique_save_to_file(FILE* file, int graph_size, stats* s);

FILE* stats_subgraph_open_csv();
void stats_subgraph_save_to_file(FILE* file, int g1_size, int g2_size, stats* s);

#endif