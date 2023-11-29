#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "stats.h"
#include "utils.h"

stats* stats_create(clock_t exact_time, clock_t approx_time) {
    stats* s = (stats*)malloc(sizeof(stats));
    if (!s) ERR("malloc");

    s->exact_time = (double)exact_time / CLOCKS_PER_SEC;
    s->approx_time = (double)approx_time / CLOCKS_PER_SEC;

    return s;
}

FILE* stats_clique_open_csv() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char file_name[50];
    strftime(file_name, sizeof(file_name), "stats/clique_%Y-%m-%d_%H-%M-%S.csv", &tm);
    
    FILE* file = fopen(file_name, "w");
    fprintf(file, "n,exact,approx\n");

    return file;
}

void stats_clique_save_to_file(FILE* file, int graph_size, stats* s) {
    fprintf(file, "%d,%f,%f\n", graph_size, s->exact_time, s->approx_time);
}

FILE* stats_subgraph_open_csv() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char file_name[50];
    strftime(file_name, sizeof(file_name), "stats/subgraph_%Y-%m-%d_%H-%M-%S.csv", &tm);

    FILE* file = fopen(file_name, "w");
    fprintf(file, "n1,n2,exact,approx\n");

    return file;
}

void stats_subgraph_save_to_file(FILE* file, int g1_size, int g2_size, stats* s) {
    fprintf(file, "%d,%d,%f,%f\n", g1_size, g2_size, s->exact_time, s->approx_time);
}
