#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "approx_clique.h"
#include "approx_subgraph.h"
#include "exact_clique.h"
#include "exact_subgraph.h"
#include "exact_clique_bb.h"
#include "bma.h"
#include "utils.h"
#include "metric.h"

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

    graph_print(g0);

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
    
    graph_print(g0);
    graph_print(g1);
    graph_print(g2);
    
    graph_save_to_file(g0, "res/TEST_BASIC_0.txt");
    graph_save_to_file(g1, "res/TEST_BASIC_1.txt");
    graph_save_to_file(g2, "res/TEST_BASIC_2.txt");
    
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
    
    printf("\nSubgraph:");
    graph_print(g0);
    printf("\nOriginal a:");
    graph_print(g0);
    printf("\nExact subgraph of a:");
    graph_print(a_exact);
    printf("\nApprox subgraph of a:");
    graph_print(a_approx);
    printf("\nOriginal b:");
    graph_print(g1);
    printf("\nExact subgraph of b:");
    graph_print(b_exact);
    printf("\nApprox subgraph of b:");
    graph_print(b_approx);


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

#pragma region Clique

void test_approx_clique(int* passed, int* failed){
    printf("\n%s\n", __func__);

    const int clique_size = 8;
    const int graph_size = 12;
    matrix* g0 = matrix_init(clique_size);
    graph_generate(g0, 3, 1, 1.0, 0);

    matrix* g2 = matrix_init(graph_size);
    graph_generate(g2, 3, 1, 0.8f, 0);

    matrix* g3 = matrix_init(graph_size);
    matrix_overload(g0, g2, g3); // now g3 has a guaranteed clique of size equal to g0's vertex count
    graph_permute(g3);

    //matrix* g4 = matrix_clone(g3);
    matrix* g4 = approx_clique_run(g3);

    graph_print(g3);
    graph_print(g4);

    graph_save_to_file(g3, "res/TEST_APPROX_CLIQUE_3.txt");
    graph_save_to_file(g4, "res/TEST_APPROX_CLIQUE_4.txt");
    
    matrix_destroy(g0);
    matrix_destroy(g2);
    matrix_destroy(g3);
    matrix_destroy(g4);
}

void test_exact_clique(int* passed, int* failed){
    printf("\n%s\n", __func__);

    const int clique_size = 6;
    const int graph_size = 12;
    matrix* g0 = matrix_init(clique_size);
    graph_generate(g0, 3, 1, 1.0, 0);

    matrix* g2 = matrix_init(graph_size);
    graph_generate(g2, 3, 1, 0.2f, 0);

    matrix* g3 = matrix_init(graph_size);
    matrix_overload(g0, g2, g3); // now g3 has a guaranteed clique of size equal to g0's vertex count
    graph_permute(g3);

    matrix* g4 = matrix_clone(g3);
    approx_clique_run(g4);

    graph_print(g3);
    graph_print(g4);

    graph_save_to_file(g3, "res/TEST_APPROX_CLIQUE_3.txt");
    graph_save_to_file(g4, "res/TEST_APPROX_CLIQUE_4.txt");
    
    matrix_destroy(g0);
    matrix_destroy(g2);
    matrix_destroy(g3);
    matrix_destroy(g4);
}

void test_exact_clique_bb(int* passed, int* failed){
    printf("\n%s\n", __func__);

    const int clique_size = 5;
    const int graph_size = 12;
    matrix* g0 = matrix_init(clique_size);
    graph_generate(g0, 1, 1, 1.0, 0);

    matrix* g2 = matrix_init(graph_size);
    graph_generate(g2, 10, 1, 0.4f, 0);

    matrix* g3 = matrix_init(graph_size);
    matrix_overload(g0, g2, g3); // now g3 has a guaranteed clique of size equal to g0's vertex count
    graph_permute(g3);

    matrix* g4 = matrix_clone(g3);
    exact_clique_bb_run(g4);

    graph_print(g3);
    graph_print(g4);

    graph_save_to_file(g3, "res/TEST_EXACT_CLIQUE_BB_3.txt");
    graph_save_to_file(g4, "res/TEST_EXACT_CLIQUE_BB_4.txt");
    
    matrix_destroy(g0);
    matrix_destroy(g2);
    matrix_destroy(g3);
    matrix_destroy(g4);
}

void test_exact_clique_bb_random(int* passed, int* failed){
    printf("\n%s\n", __func__);

    const int graph_size = 10;

    matrix* g0 = matrix_init(graph_size);
    graph_generate(g0, 10, 1, 0.2f, 0);
    graph_permute(g0);

    matrix* g1 = matrix_clone(g0);
    exact_clique_bb_run(g1);

    graph_print(g0);
    graph_print(g1);

    graph_save_to_file(g0, "res/TEST_EXACT_CLIQUE_BB_RANDOM_0.txt");
    graph_save_to_file(g1, "res/TEST_EXACT_CLIQUE_BB_RANDOM_1.txt");
    
    matrix_destroy(g0);
    matrix_destroy(g1);
}

void test_clique(int* passed, int* failed) {
    printf("\n%s\n", __func__);

    const int clique_size = 50;
    const int graph_size = 500;
    matrix* g0 = matrix_init(clique_size);
    graph_generate(g0, 3, 1, 1.0, 0);

    matrix* g2 = matrix_init(graph_size);
    graph_generate(g2, 3, 1, 0.2f, 0);

    matrix* g3 = matrix_init(graph_size);
    matrix_overload(g0, g2, g3); // now g3 has a guaranteed clique of size equal to g0's vertex count
    graph_permute(g3);

    matrix* g_exact = matrix_clone(g3);

    clock_t time_exact = clock();
    exact_clique_bb_run(g_exact);
    time_exact = clock() - time_exact;

    clock_t time_approx = clock();
    matrix* g_approx = approx_clique_run(g0);
    time_approx = clock() - time_approx;

    graph_print(g3);
    graph_print(g_exact);
    graph_print(g_approx);

    utils_print_execution_time(time_exact, time_approx);

    graph_save_to_file(g3, "res/TEST_EXACT_CLIQUE_3.txt");
    graph_save_to_file(g_exact, "res/TEST_EXACT_CLIQUE_EXACT.txt");
    graph_save_to_file(g_approx, "res/TEST_EXACT_CLIQUE_APPROX.txt");

    if (graph_clique_equal(g_exact, g_approx)) {
        printf("\033[0;32m%s", "\nPASS");
    }
    else {
        printf("\033[0;31m%s", "\nFAIL");
    }

    printf("\033[0;37m\n\n");

    matrix_destroy(g0);
    matrix_destroy(g2);
    matrix_destroy(g3);
    matrix_destroy(g_exact);
    matrix_destroy(g_approx);
}

void test_clique_random(int* passed, int* failed) {
    printf("\n%s\n", __func__);

    const int directed = 1;
    const int graph_size = 200;

    matrix* g0 = matrix_init(graph_size);
    graph_generate(g0, 5, 1, 0.5f, directed);
    
    if (directed) {
        graph_simplify_multidigraph_to_multigraph(g0);
    }
    
    graph_permute(g0);

    matrix* g_exact = matrix_clone(g0);
    
    clock_t time_exact = clock();
    exact_clique_bb_run(g_exact);
    time_exact = clock() - time_exact;

    clock_t time_approx = clock();
    matrix* g_approx = approx_clique_run(g0);
    time_approx = clock() - time_approx;

    graph_print(g0);
    graph_print(g_exact);
    graph_print(g_approx);

    utils_print_execution_time(time_exact, time_approx);

    graph_save_to_file(g0, "res/TEST_EXACT_CLIQUE_3.txt");
    graph_save_to_file(g_exact, "res/TEST_EXACT_CLIQUE_EXACT.txt");
    graph_save_to_file(g_approx, "res/TEST_EXACT_CLIQUE_APPROX.txt");

    if (graph_clique_equal(g_exact, g_approx)) {
        printf("\033[0;32m%s", "\nPASS");
    }
    else {
        printf("\033[0;31m%s", "\nFAIL");
    }

    printf("\033[0;37m\n\n");
    
    printf("Exact clique size: %d\n", graph_calc_clique_size(g_exact));
    printf("Approx clique size: %d\n", graph_calc_clique_size(g_approx));

    matrix_destroy(g0);
    matrix_destroy(g_exact);
    matrix_destroy(g_approx);
}

#pragma endregion


#pragma region Subgraph

void test_exact_subgraph_simple(int* passed, int* failed){
    printf("\n%s\n", __func__);

    const int subgraph_size = 3;
    matrix* g0 = matrix_init(subgraph_size);
    g0->mat[0 * 3 + 1] = 1;
    g0->mat[1 * 3 + 2] = 1;
    g0->mat[0 * 3 + 2] = 1;
    
    matrix* g1 = matrix_extend(g0, 1);
    graph_permute(g1);

    matrix* g2 = matrix_clone(g0);
    matrix* g3 = matrix_clone(g1);
    exact_subgraph_run(g2, g3);

    graph_print(g0);
    graph_print(g1);
    graph_print(g2);
    graph_print(g3);

    graph_save_to_file(g0, "res/TEST_EXACT_SUBGRAPH_SIMPLE_0.txt");
    graph_save_to_file(g1, "res/TEST_EXACT_SUBGRAPH_SIMPLE_1.txt");
    graph_save_to_file(g2, "res/TEST_EXACT_SUBGRAPH_SIMPLE_2.txt");
    graph_save_to_file(g3, "res/TEST_EXACT_SUBGRAPH_SIMPLE_3.txt");
    
    matrix_destroy(g0);
    matrix_destroy(g1);
    matrix_destroy(g2);
    matrix_destroy(g3);
}

void test_exact_subgraph(int* passed, int* failed){
    printf("\n%s\n", __func__);

    const int subgraph_size = 2;
    const int graph_a_size = 3;
    const int graph_b_size = 3;
    matrix* g0 = matrix_init(subgraph_size);
    graph_generate(g0, 7, 7, 0.7f, 1);
    

    matrix* g3 = matrix_init(graph_a_size);
    graph_generate(g3, 2, 2, 0.0, 1);
    matrix* g4 = matrix_init(graph_b_size);
    graph_generate(g4, 3, 3, 1.0, 1);

    matrix* g5 = matrix_init(graph_a_size);
    matrix_overload(g0, g3, g5); 
    matrix* g6 = matrix_init(graph_b_size);
    matrix_overload(g0, g4, g6); // now g5 and g6 have a guaranteed g0 subgraph

    graph_permute(g5);
    graph_permute(g6);

    matrix* g7 = matrix_clone(g5);
    matrix* g8 = matrix_clone(g6);
    exact_subgraph_run(g7, g8);

    graph_print(g0);
    graph_print(g5);
    graph_print(g7);
    graph_print(g6);
    graph_print(g8);

    graph_save_to_file(g0, "res/TEST_EXACT_SUBGRAPH_5.txt");
    graph_save_to_file(g5, "res/TEST_EXACT_SUBGRAPH_5.txt");
    graph_save_to_file(g6, "res/TEST_EXACT_SUBGRAPH_6.txt");
    graph_save_to_file(g7, "res/TEST_EXACT_SUBGRAPH_7.txt");
    graph_save_to_file(g8, "res/TEST_EXACT_SUBGRAPH_8.txt");
    
    matrix_destroy(g0);
    matrix_destroy(g3);
    matrix_destroy(g4);
    matrix_destroy(g5);
    matrix_destroy(g6);
    matrix_destroy(g7);
    matrix_destroy(g8);
}

void test_approx_subgraph(int* passed, int* failed){
    printf("\n%s\n", __func__);

    const int subgraph_size = 4;
    const int graph_a_size = 6;
    const int graph_b_size = 7;
    matrix* g0 = matrix_init(subgraph_size);
    graph_generate(g0, 3, 1, 0.7f, 1);
    

    matrix* g3 = matrix_init(graph_a_size);
    graph_generate(g3, 3, 1, 0.8f, 1);
    matrix* g4 = matrix_init(graph_b_size);
    graph_generate(g4, 5, 2, 0.6f, 1);

    matrix* g5 = matrix_init(graph_a_size);
    matrix_overload(g0, g3, g5); 
    matrix* g6 = matrix_init(graph_b_size);
    matrix_overload(g0, g4, g6); // now g5 and g6 have a guaranteed g0 subgraph

    graph_permute(g5);
    graph_permute(g6);

    matrix* g7 = matrix_clone(g5);
    matrix* g8 = matrix_clone(g6);
    approx_subgraph_run(g7, g8);

    graph_print(g5);
    graph_print(g6);
    graph_print(g7);
    graph_print(g8);

    graph_save_to_file(g5, "res/TEST_APPROX_SUBGRAPH_5.txt");
    graph_save_to_file(g6, "res/TEST_APPROX_SUBGRAPH_6.txt");
    graph_save_to_file(g7, "res/TEST_APPROX_SUBGRAPH_7.txt");
    graph_save_to_file(g7, "res/TEST_APPROX_SUBGRAPH_8.txt");
    
    matrix_destroy(g0);
    matrix_destroy(g3);
    matrix_destroy(g4);
    matrix_destroy(g5);
    matrix_destroy(g6);
    matrix_destroy(g7);
    matrix_destroy(g8);
    
}

void test_subgraph(int* passed, int* failed) {
    printf("\n%s\n", __func__);

    const int subgraph_size = 5;
    const int graph_a_size = 12;
    const int graph_b_size = 9;
    matrix* g0 = matrix_init(subgraph_size);
    graph_generate(g0, 7, 1, 0.8f, 1);


    matrix* g3 = matrix_init(graph_a_size);
    graph_generate(g3, 2, 1, 0.0, 1);
    matrix* g4 = matrix_init(graph_b_size);
    graph_generate(g4, 3, 1, 1.0, 1);

    matrix* g5 = matrix_init(graph_a_size);
    matrix_overload(g0, g3, g5);
    matrix* g6 = matrix_init(graph_b_size);
    matrix_overload(g0, g4, g6); // now g5 and g6 have a guaranteed g0 subgraph

    graph_permute(g5);
    graph_permute(g6);

    matrix* a_exact = matrix_clone(g5);
    matrix* b_exact = matrix_clone(g6);

    clock_t time_exact = clock();
    exact_subgraph_run(a_exact, b_exact);
    time_exact = clock() - time_exact;

    matrix* a_approx = matrix_clone(g5);
    matrix* b_approx = matrix_clone(g6);

    clock_t time_approx = clock();
    approx_subgraph_run(a_approx, b_approx);
    time_approx = clock() - time_approx;

    printf("\nSubgraph:");
    graph_print(g0);
    printf("\nOriginal a:");
    graph_print(g5);
    printf("\nExact subgraph of a:");
    graph_print(a_exact);
    printf("\nApprox subgraph of a:");
    graph_print(a_approx);
    printf("\nOriginal b:");
    graph_print(g6);
    printf("\nExact subgraph of b:");
    graph_print(b_exact);
    printf("\nApprox subgraph of b:");
    graph_print(b_approx);

    utils_print_execution_time(time_exact, time_approx);

    if (distance(g0, a_exact) == 0 &&
        distance(g0, b_exact) == 0) 
    {
        printf("\033[0;32m%s", "\nPASSED");
        printf("\033[0;37m\n\n");
        (*passed)++;
        
        if (
            distance(a_exact, a_approx) != 0 ||
            distance(b_exact, b_approx) != 0
            ) 
        {
            printf("\033[0;31m%s", "\nAPPROX FAILED");
        }
    }
    else
    {
        printf("\033[0;31m%s", "\nFAIL");
        (*failed)++;
    }
    

    printf("\033[0;37m\n\n");

    graph_save_to_file(g0, "res/TEST_EXACT_SUBGRAPH_5.txt");
    graph_save_to_file(g5, "res/TEST_EXACT_SUBGRAPH_5.txt");
    graph_save_to_file(g6, "res/TEST_EXACT_SUBGRAPH_6.txt");
    graph_save_to_file(a_exact, "res/TEST_EXACT_SUBGRAPH_7.txt");
    graph_save_to_file(a_approx, "res/TEST_EXACT_SUBGRAPH_7.txt");
    graph_save_to_file(b_exact, "res/TEST_EXACT_SUBGRAPH_8.txt");
    graph_save_to_file(b_approx, "res/TEST_EXACT_SUBGRAPH_8.txt");

    matrix_destroy(g0);
    matrix_destroy(g3);
    matrix_destroy(g4);
    matrix_destroy(g5);
    matrix_destroy(g6);
    matrix_destroy(a_exact);
    matrix_destroy(a_approx);
    matrix_destroy(b_exact);
    matrix_destroy(b_approx);
}

#pragma endregion

#pragma region Metric

void test_metric_pass(int* passed, int* failed) {
    int size = 15;
    matrix* g1 = matrix_init(size);
    graph_generate(g1, 7, 1, 0.8f, 1);
    matrix* g2 = matrix_clone(g1);

    int d = distance(g1, g2);

    printf("G1\n");
    graph_print(g1);
    printf("G2\n");
    graph_print(g2);
    printf("Distance: %d", d);

    if (d == 0) {
        printf("\033[0;32m%s", "\nPASS");
        (*passed)++;
    }
    else {
        printf("\033[0;31m%s", "\nFAIL");
        (*failed)++;
    }

    printf("\033[0;37m\n\n");

    matrix_destroy(g1);
    matrix_destroy(g2);
}

void test_metric_permute_pass(int* passed, int* failed) {
    int size = 20;
    matrix* g1 = matrix_init(size);
    graph_generate(g1, 1, 0, 0.8f, 1);
    matrix* g2 = matrix_clone(g1);
    graph_permute(g2);

    printf("G1\n");
    graph_print(g1);
    printf("G2\n");
    graph_print(g2);

    int d = distance(g1, g2);

    printf("Distance: %d", d);

    if (d == 0) {
        printf("\033[0;32m%s", "\nPASS");
        (*passed)++;
    }
    else {
        printf("\033[0;31m%s", "\nFAIL");
        (*failed)++;
    }

    printf("\033[0;37m\n\n");

    matrix_destroy(g1);
    matrix_destroy(g2);
}

void test_metric_fail(int* passed, int* failed) {
    int size = 11;
    matrix* g1 = matrix_init(size);
    graph_generate(g1, 7, 1, 0.8f, 1);
    matrix* g2 = matrix_clone(g1);

    graph_add_noise(g2, 0.5f, 1, 0.0);

    int d = distance(g1, g2);

    printf("G1\n");
    graph_print(g1);
    printf("G2\n");
    graph_print(g2);
    printf("Distance: %d", d);

    if (d != 0) {
        printf("\033[0;32m%s", "\nPASS");
        (*passed)++;
    }
    else {
        printf("\033[0;31m%s", "\nFAIL");
        (*failed)++;
    }

    printf("\033[0;37m\n\n");

    matrix_destroy(g1);
    matrix_destroy(g2);
}

#pragma endregion

int main() {
    srand((unsigned int)time(NULL));

    int passed = 0, failed = 0;

    //test_metric_fail(&passed, &failed);
    //test_metric_pass(&passed, &failed);
    //test_metric_permute_pass(&passed, &failed);

    //test_approx_clique_v1();
    //test_approx_clique();

    //test_clique();
    //test_clique_random();

    //test_subgraph_simple();
    test_subgraph(&passed, &failed);

    //test_basic();
    //test_approx_clique();
    //test_approx_subgraph();
    //test_exact_clique();
    //test_exact_subgraph_simple();
    //test_exact_subgraph();
    //test_exact_clique_bb();
    //test_exact_clique_bb_random();


    printf("-------------\n");
    printf("TESTS SUMMARY\n");
    printf("-------------\n");
    printf("\033[0;32m%s: %d\n", "PASSED", passed);
    printf("\033[0;31m%s: %d\n", "FAILED", failed);
    printf("\033[0;37m");
    printf("-------------\n");

    return 0;
}
