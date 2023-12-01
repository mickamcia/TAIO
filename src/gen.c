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
int graph_size(matrix* g){
    int size = 0;
    for(int i = 0; i < g->size; i++){
        for(int j = 0; j < g->size; j++){
            if(g->mat[i * g->size + j] > 0){
                size += g->mat[i * g->size + j];
            }
        }
    }
    return size;
}
void quick_test(){
    while(1){
        matrix* g1_original = matrix_init(2);
        matrix* g2_original = matrix_init(3);
        graph_generate(g1_original, 10, 0, 0.5, 1);
        graph_generate(g2_original, 10, 0, 0.5, 1);
        matrix* g1_first = matrix_clone(g1_original);
        matrix* g2_first = matrix_clone(g2_original);
        matrix* g1_second = matrix_clone(g1_original);
        matrix* g2_second = matrix_clone(g2_original);
        exact_subgraph_run(g1_first, g2_first);
        approx_subgraph_run(g1_second, g2_second);
        if(graph_size(g1_first) != graph_size(g2_first)
        || graph_size(g1_first) != graph_size(g1_second) //check equality between approx and exact
        || graph_size(g1_second) != graph_size(g2_second)
        )
        {
            printf("AAAAAAAAAAAAAAAAAAAAA\n");
            graph_print(g1_original, "g1_original");
            graph_print(g2_original, "g2_original");
            graph_print(g1_first, "g1_first_exact");
            graph_print(g1_second, "g1_second_approx");
            graph_print(g2_first, "g2_first_exact");
            graph_print(g2_second, "g2_second_approx");
        }
        matrix_destroy(g1_first);
        matrix_destroy(g1_second);
        matrix_destroy(g2_first);
        matrix_destroy(g2_second);
        matrix_destroy(g1_original);
        matrix_destroy(g2_original);

    }
}
int main(int argc, char** argv) {
    //srand((unsigned int)time(NULL));
    srand(985); // 30 sec, 0 fails
    //srand(12724); // 630 sec, 0 fails
    //srand(54322); // 125 sec, 2 fails
    quick_test();

    bool run_distance;
    bool run_clique;
    bool run_subgraph;
    bool generate_stats = false;
    matrix* g1 = NULL;
    matrix* g2 = NULL;

    read_args(argc, argv, &run_distance, &run_clique, &run_subgraph, &generate_stats, &g1, &g2);

    int passed = 0, failed = 0, approx_failed = 0;

    if (generate_stats) {
        clock_t tests_time = clock();

        test_clique_stats(&passed, &failed, &approx_failed);
        test_subgraph_stats(&passed, &failed, &approx_failed);
        
        tests_time = clock() - tests_time;
        print_tests_summary(passed, failed, approx_failed, tests_time);
        return 0;
    }

    if (run_distance) {
        test_metric_from_args(g1, g2, &passed, &failed);
    }

    if (run_clique) {
        test_clique_from_args(g1, &passed, &failed, &approx_failed);
    }

    if (run_subgraph) {
        test_subgraph_from_args(g1, g2, &passed, &failed, &approx_failed);
    }

    if (!run_distance && !run_clique && !run_subgraph) {
        clock_t tests_time = clock();
        tests_metric(&passed, &failed);
        tests_clique(&passed, &failed, &approx_failed);
        tests_subgraph(&passed, &failed, &approx_failed);
        tests_time = clock() - tests_time;

        print_tests_summary(passed, failed, approx_failed, tests_time);
    }

    if (g1 != NULL) free(g1);
    if (g2 != NULL) free(g2);

    return 0;
}
