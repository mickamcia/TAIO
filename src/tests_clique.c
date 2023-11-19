#include "tests.h"

void test_approx_clique(int* passed, int* failed) {
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

    graph_print(g3, "");
    graph_print(g4, "");

    graph_save_to_file(g3, "res/TEST_APPROX_CLIQUE_3.txt");
    graph_save_to_file(g4, "res/TEST_APPROX_CLIQUE_4.txt");

    matrix_destroy(g0);
    matrix_destroy(g2);
    matrix_destroy(g3);
    matrix_destroy(g4);
}

void test_exact_clique(int* passed, int* failed) {
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

    graph_print(g3, "Exact clique");
    graph_print(g4, "Approximated clique");

    graph_save_to_file(g3, "res/TEST_APPROX_CLIQUE_3.txt");
    graph_save_to_file(g4, "res/TEST_APPROX_CLIQUE_4.txt");

    matrix_destroy(g0);
    matrix_destroy(g2);
    matrix_destroy(g3);
    matrix_destroy(g4);
}

void test_exact_clique_bb(int* passed, int* failed) {
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

    graph_print(g3,"");
    graph_print(g4,"");

    graph_save_to_file(g3, "res/TEST_EXACT_CLIQUE_BB_3.txt");
    graph_save_to_file(g4, "res/TEST_EXACT_CLIQUE_BB_4.txt");

    matrix_destroy(g0);
    matrix_destroy(g2);
    matrix_destroy(g3);
    matrix_destroy(g4);
}

void test_exact_clique_bb_random(int* passed, int* failed) {
    printf("\n%s\n", __func__);

    const int graph_size = 10;

    matrix* g0 = matrix_init(graph_size);
    graph_generate(g0, 10, 1, 0.2f, 0);
    graph_permute(g0);

    matrix* g1 = matrix_clone(g0);
    exact_clique_bb_run(g1);

    graph_print(g0, "");
    graph_print(g1, "");

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

    //graph_print(g3);
    graph_print(g_exact, "Exact clique");
    graph_print(g_approx, "Approximated clique");

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

    //graph_print(g0);
    graph_print(g_exact, "Exact clique");
    graph_print(g_approx, "Approximated clique");

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

void tests_clique(int* passed, int* failed) {

    printf("Running clique tests...\n");


}

