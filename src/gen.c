#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "graph.h"
#include "approx_clique.h"
#include "approx_subgraph.h"
#include "exact_clique.h"
#include "exact_subgraph.h"
#include "exact_clique_bb.h"
#include "bma.h"
#include "utils.h"
#include "metric.h"
#include "tests.h"

#pragma region Examples to be removed

void paper_example(matrix* g0) {
    // graf z papieru o bellmanie-fordzie

    int a = 0, b = 1, c = 2, d = 3, e = 4, f = 5, g = 6, h = 7;
    int size = g0->size;

    for (int i = 0; i < g0->size; i++)
        for (int j = 0; j < g0->size; j++)
            g0->mat[i * size + j] = 1;

    g0->mat[a * size + b] = 0;
    g0->mat[a * size + c] = 0;
    g0->mat[a * size + h] = 0;
    g0->mat[b * size + a] = 0;
    g0->mat[b * size + e] = 0;
    g0->mat[c * size + a] = 0;
    g0->mat[c * size + d] = 0;
    g0->mat[d * size + c] = 0;
    g0->mat[d * size + g] = 0;
    g0->mat[d * size + f] = 0;
    g0->mat[e * size + b] = 0;
    g0->mat[e * size + g] = 0;
    g0->mat[e * size + f] = 0;
    g0->mat[f * size + h] = 0;
    g0->mat[f * size + d] = 0;
    g0->mat[f * size + e] = 0;
    g0->mat[g * size + h] = 0;
    g0->mat[g * size + d] = 0;
    g0->mat[g * size + e] = 0;
    g0->mat[h * size + g] = 0;
    g0->mat[h * size + f] = 0;
    g0->mat[h * size + a] = 0;
}
void medium_example(matrix* g) {
    int a = 0, b = 1, c = 2, d = 3, e = 4, f = 5;

    g->mat[a * g->size + b] = 1;
    g->mat[a * g->size + f] = 1;
    g->mat[b * g->size + a] = 1;
    g->mat[b * g->size + f] = 1;
    g->mat[f * g->size + a] = 1;
    g->mat[f * g->size + b] = 1;
    g->mat[f * g->size + c] = 1;
    g->mat[c * g->size + f] = 1;
    g->mat[c * g->size + b] = 1;
    g->mat[c * g->size + a] = 1;
    g->mat[a * g->size + c] = 1;
    g->mat[b * g->size + c] = 1;

    g->mat[d * g->size + e] = 5;
    g->mat[e * g->size + d] = 2;
}
void simple_example(matrix* g0) {
    int a = 0, b = 1, c = 2;
    int size = g0->size;

    g0->mat[a * size + c] = 2;
    g0->mat[c * size + a] = 1;
}
void example2(matrix* g0) {
    int a = 0, b = 1, c = 2, d = 3;
    int size = g0->size;

    g0->mat[a * size + b] = 3;
    g0->mat[a * size + c] = 3;
    g0->mat[a * size + d] = 3;

    g0->mat[b * size + a] = 1;
    g0->mat[c * size + a] = 1;
    g0->mat[d * size + a] = 1;
}
void example22(matrix* g0) {
    int a = 0, b = 1, c = 2;
    int size = g0->size;

    g0->mat[a * size + b] = 10;
    //g0->mat[a * size + c] = 3;

    g0->mat[b * size + a] = 4;
    //g0->mat[c * size + a] = 1;
}
void test_approx_clique_v1() {
    int size = 6;

    matrix* g0 = matrix_init(size);
   
    //paper_example(g0);
    medium_example(g0);
    //simple_example(g0);

    graph_print(g0, "");

    approx_clique_run(g0);

    matrix_destroy(g0);
}
void test_basic(){
    printf("\n%s\n", __func__);

    matrix* g0 = matrix_init(8);
    graph_generate(g0, 5, 2, 0.5, 0);

    matrix* g1 = matrix_clone(g0);
    graph_add_noise(g1, 0.5, 1, 0.0);

    matrix* g2 = matrix_clone(g1);
    graph_permute(g2);
    
    graph_print(g0, "");
    graph_print(g1, "");
    graph_print(g2, "");
    
    /*graph_save_to_file(g0, "res/TEST_BASIC_0.txt");
    graph_save_to_file(g1, "res/TEST_BASIC_1.txt");
    graph_save_to_file(g2, "res/TEST_BASIC_2.txt");*/
    
    matrix_destroy(g0);
    matrix_destroy(g1);
    matrix_destroy(g2);
}

void test_subgraph_simple() {
    int size = 4;

    matrix* g0 = matrix_init(size);
    example2(g0);

    matrix* g1 = matrix_init(size );
    example2(g1);

    graph_simplify_multidigraph_to_multigraph(g1);

    matrix* a_exact = matrix_clone(g0);
    matrix* b_exact = matrix_clone(g1);
    matrix* a_approx = matrix_clone(g0);
    matrix* b_approx = matrix_clone(g1);

    exact_subgraph_run(a_exact, b_exact);
    approx_subgraph_run(a_approx, b_approx);
    
    graph_print(g0, "Subgraph");
    graph_print(g0, "Original a");
    graph_print(a_exact, "Exact subgraph of a");
    graph_print(a_approx, "Approx subgraph of a");
    graph_print(g1, "Original b");
    graph_print(b_exact, "Exact subgraph of b");
    graph_print(b_approx, "Approx subgraph of b");


    if (graph_clique_equal(a_exact, a_approx) && graph_clique_equal(b_exact, b_approx)) {
        printf("\033[0;32m%s", "\nPASS");
    }
    else {
        printf("\033[0;31m%s", "\nFAIL");
    }

    printf("\033[0;37m\n\n");

    matrix_destroy(g0);
    matrix_destroy(g1);
}

#pragma endregion

int main(int argc, char** argv) {
    //srand((unsigned int)time(NULL));
    srand(985); // 30 sec, 0 fails
    //srand(12724); // 630 sec, 0 fails
    //srand(54322); // 125 sec, 2 fails

    bool run_distance;
    bool run_clique;
    bool run_subgraph;
    bool generate_stats = false;
    matrix* g1 = NULL;
    matrix* g2 = NULL;

    read_args(argc, argv, &run_distance, &run_clique, &run_subgraph, &generate_stats, &g1, &g2);

    int passed = 0, failed = 0;

    if (generate_stats) {
        clock_t tests_time = clock();

        test_clique_stats(&passed, &failed);
        test_subgraph_stats(&passed, &failed);
        
        tests_time = clock() - tests_time;
        print_tests_summary(passed, failed, tests_time);
        return 0;
    }

    if (run_distance) {
        test_metric_from_args(g1, g2, &passed, &failed);
    }

    if (run_clique) {
        test_clique_from_args(g1, &passed, &failed);
    }

    if (run_subgraph) {
        test_subgraph_from_args(g1, g2, &passed, &failed);
    }

    if (!run_distance && !run_clique && !run_subgraph) {
        clock_t tests_time = clock();
        tests_metric(&passed, &failed);
        tests_clique(&passed, &failed);
        tests_subgraph(&passed, &failed);
        tests_time = clock() - tests_time;

        print_tests_summary(passed, failed, tests_time);
    }

    if (g1 != NULL) free(g1);
    if (g2 != NULL) free(g2);

    return 0;
}
