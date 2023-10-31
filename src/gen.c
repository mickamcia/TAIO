#include "graph.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "approx_clique.h"
#include "approx_subgraph.h"
#include "exact_clique.h"
#include "exact_subgraph.h"

void test_basic(){
    matrix* g0 = matrix_init(8);
    graph_generate(g0, 5, 2, 0.5);

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
    graph_generate(g0, 3, 1, 1.0);
    matrix* g1 = matrix_extend(g0, graph_size - clique_size);

    matrix* g2 = matrix_init(graph_size);
    graph_generate(g2, 3, 1, 0.8);

    matrix* g3 = matrix_init(graph_size);
    matrix_add(g1, g2, g3); // now g3 has a guaranteed clique of size equal to g0's vertex count
    graph_permute(g3);

    matrix* g4 = approx_clique_run(g3);

    graph_print(g3);
    graph_print(g4);

    graph_save_to_file(g3, "res/TEST_APPROX_CLIQUE_3.txt");
    graph_save_to_file(g4, "res/TEST_APPROX_CLIQUE_4.txt");
    
    matrix_destroy(g0);
    matrix_destroy(g1);
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
    graph_generate(g0, 3, 1, 0.7);
    
    matrix* g1 = matrix_extend(g0, graph_a_size - subgraph_size);
    matrix* g2 = matrix_extend(g0, graph_b_size - subgraph_size);

    matrix* g3 = matrix_init(graph_a_size);
    graph_generate(g2, 3, 1, 0.8);
    matrix* g4 = matrix_init(graph_b_size);
    graph_generate(g3, 5, 2, 0.6);

    matrix* g5 = matrix_init(graph_a_size);
    matrix_add(g1, g3, g5); 
    matrix* g6 = matrix_init(graph_b_size);
    matrix_add(g2, g4, g6); // now g5 and g6 have a guaranteed g0 subgraph

    graph_permute(g5);
    graph_permute(g6);

    matrix* g7 = approx_subgraph_run(g5, g6);

    graph_print(g5);
    graph_print(g6);
    graph_print(g7);

    graph_save_to_file(g5, "res/TEST_APPROX_SUBGRAPH_5.txt");
    graph_save_to_file(g6, "res/TEST_APPROX_SUBGRAPH_6.txt");
    graph_save_to_file(g7, "res/TEST_APPROX_SUBGRAPH_7.txt");
    
    matrix_destroy(g0);
    matrix_destroy(g1);
    matrix_destroy(g2);
    matrix_destroy(g3);
    matrix_destroy(g4);
    matrix_destroy(g5);
    matrix_destroy(g6);
    matrix_destroy(g7);
    
}

void test_exact_clique(){
    printf("\n%s\n", __func__);

    const int clique_size = 8;
    const int graph_size = 20;
    matrix* g0 = matrix_init(clique_size);
    graph_generate(g0, 3, 1, 1.0);
    matrix* g1 = matrix_extend(g0, graph_size - clique_size);

    matrix* g2 = matrix_init(graph_size);
    graph_generate(g2, 3, 1, 0.8);

    matrix* g3 = matrix_init(graph_size);
    matrix_add(g1, g2, g3); // now g3 has a guaranteed clique of size equal to g0's vertex count
    graph_permute(g3);

    matrix* g4 = exact_clique_run(g3);

    graph_print(g3);
    graph_print(g4);

    graph_save_to_file(g3, "res/TEST_EXACT_CLIQUE_3.txt");
    graph_save_to_file(g4, "res/TEST_EXACT_CLIQUE_4.txt");
    
    matrix_destroy(g0);
    matrix_destroy(g1);
    matrix_destroy(g2);
    matrix_destroy(g3);
    matrix_destroy(g4);
}

void test_exact_subgraph(){
    printf("\n%s\n", __func__);

    const int subgraph_size = 4;
    const int graph_a_size = 6;
    const int graph_b_size = 7;
    matrix* g0 = matrix_init(subgraph_size);
    graph_generate(g0, 3, 1, 0.7);
    
    matrix* g1 = matrix_extend(g0, graph_a_size - subgraph_size);
    matrix* g2 = matrix_extend(g0, graph_b_size - subgraph_size);

    matrix* g3 = matrix_init(graph_a_size);
    graph_generate(g2, 3, 1, 0.8);
    matrix* g4 = matrix_init(graph_b_size);
    graph_generate(g3, 5, 2, 0.6);

    matrix* g5 = matrix_init(graph_a_size);
    matrix_add(g1, g3, g5); 
    matrix* g6 = matrix_init(graph_b_size);
    matrix_add(g2, g4, g6); // now g5 and g6 have a guaranteed g0 subgraph

    graph_permute(g5);
    graph_permute(g6);

    matrix* g7 = exact_subgraph_run(g5, g6);

    graph_print(g5);
    graph_print(g6);
    graph_print(g7);

    graph_save_to_file(g5, "res/TEST_EXACT_SUBGRAPH_5.txt");
    graph_save_to_file(g6, "res/TEST_EXACT_SUBGRAPH_6.txt");
    graph_save_to_file(g7, "res/TEST_EXACT_SUBGRAPH_7.txt");
    
    matrix_destroy(g0);
    matrix_destroy(g1);
    matrix_destroy(g2);
    matrix_destroy(g3);
    matrix_destroy(g4);
    matrix_destroy(g5);
    matrix_destroy(g6);
    matrix_destroy(g7);
}

int main(){
    srand(time(NULL));  

    test_basic();
    test_approx_clique();
    test_approx_subgraph();
    test_exact_clique();
    test_exact_subgraph();

    return 0;
}
