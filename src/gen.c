#include "graph.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "approx_clique.h"
#include "approx_subgraph.h"
#include "exact_clique.h"
#include "exact_subgraph.h"
#include "exact_clique_bb.h"
#include "bma.h"

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

void test_approx_clique(){
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

void test_approx_subgraph(){
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

void test_exact_clique(){
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
    exact_clique_run(g4);

    graph_print(g3);
    graph_print(g4);

    graph_save_to_file(g3, "/res/TEST_EXACT_CLIQUE_3.txt");
    graph_save_to_file(g4, "/res/TEST_EXACT_CLIQUE_4.txt");
    
    matrix_destroy(g0);
    matrix_destroy(g2);
    matrix_destroy(g3);
    matrix_destroy(g4);
}

void test_exact_clique_bb(){
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

void test_exact_subgraph_simple(){
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

void test_exact_subgraph(){
    printf("\n%s\n", __func__);

    const int subgraph_size = 6;
    const int graph_a_size = 8;
    const int graph_b_size = 9;
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

void test_exact_clique_bb_random(){
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

int main(){
    srand((unsigned int)time(NULL));  

    test_approx_clique_v1();

    //test_basic();
    //test_approx_clique();
    //test_approx_subgraph();
    //test_exact_clique();
    //test_exact_subgraph_simple();
    //test_exact_subgraph();
    //test_exact_clique_bb();
    //test_exact_clique_bb_random();

    return 0;
}
