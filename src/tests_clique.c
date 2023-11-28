#include <stdlib.h>
#include "tests.h"
#include "stats.h"

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

    //graph_save_to_file(g3, "res/TEST_APPROX_CLIQUE_3.txt");
    //graph_save_to_file(g4, "res/TEST_APPROX_CLIQUE_4.txt");

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

    //graph_save_to_file(g3, "res/TEST_APPROX_CLIQUE_3.txt");
    //graph_save_to_file(g4, "res/TEST_APPROX_CLIQUE_4.txt");

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

    //graph_save_to_file(g3, "res/TEST_EXACT_CLIQUE_BB_3.txt");
    //graph_save_to_file(g4, "res/TEST_EXACT_CLIQUE_BB_4.txt");

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

    //graph_save_to_file(g0, "res/TEST_EXACT_CLIQUE_BB_RANDOM_0.txt");
    //graph_save_to_file(g1, "res/TEST_EXACT_CLIQUE_BB_RANDOM_1.txt");

    matrix_destroy(g0);
    matrix_destroy(g1);
}

void test_clique_full_graph(int* passed, int* failed) {
    const int clique_size = 50;
    const int graph_size = 50;

    printf("\nTEST: %s - n=%d, full graph\n", __func__, graph_size);

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

   /* graph_save_to_file(g3, "res/TEST_EXACT_CLIQUE_3.txt");
    graph_save_to_file(g_exact, "res/TEST_EXACT_CLIQUE_EXACT.txt");
    graph_save_to_file(g_approx, "res/TEST_EXACT_CLIQUE_APPROX.txt");*/

    printf("\nExact clique size: %d\n", graph_calc_clique_size(g_exact));
    printf("Approx clique size: %d\n", graph_calc_clique_size(g_approx));

    if (graph_clique_equal(g_exact, g_approx)) {
        print_test_pass(__func__);
        (*passed)++;
    }
    else {
        print_test_fail_msg(__func__, "approx");
        (*failed)++;
    }

    matrix_destroy(g0);
    matrix_destroy(g2);
    matrix_destroy(g3);
    matrix_destroy(g_exact);
    matrix_destroy(g_approx);
}

void test_clique_no_edges(int* passed, int* failed) {
    const int clique_size = 25;
    const int graph_size = 25;

    printf("\nTEST: %s - n=%d, empty graph\n", __func__, graph_size);

    matrix* g0 = matrix_init(clique_size);
    graph_generate(g0, 3, 1, 0.0, 0);

    matrix* g2 = matrix_init(graph_size);
    graph_generate(g2, 3, 1, 0.0f, 0);

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

    /* graph_save_to_file(g3, "res/TEST_EXACT_CLIQUE_3.txt");
     graph_save_to_file(g_exact, "res/TEST_EXACT_CLIQUE_EXACT.txt");
     graph_save_to_file(g_approx, "res/TEST_EXACT_CLIQUE_APPROX.txt");*/

    printf("\nExact clique size: %d\n", graph_calc_clique_size(g_exact));
    printf("Approx clique size: %d\n", graph_calc_clique_size(g_approx));

    if (graph_clique_equal(g_exact, g_approx)) {
        print_test_pass(__func__);
        (*passed)++;
    }
    else {
        print_test_fail_msg(__func__, "approx");
        (*failed)++;
    }

    matrix_destroy(g0);
    matrix_destroy(g2);
    matrix_destroy(g3);
    matrix_destroy(g_exact);
    matrix_destroy(g_approx);
}

stats* test_clique_random(int graph_size, int* passed, int* failed) {
    const int directed = 1;
    //const int graph_size = 200;
    
    printf("\nTEST: %s - n: %d\n", __func__, graph_size);

    matrix* g0 = matrix_init(graph_size);
    graph_generate(g0, 15, 1, 0.45f, directed);

    matrix* g_exact = matrix_clone(g0);

    clock_t time_exact = clock();
    exact_clique_bb_run(g_exact);
    time_exact = clock() - time_exact;

    clock_t time_approx = clock();
    matrix* g_approx = approx_clique_run(g0);
    time_approx = clock() - time_approx;

    graph_print(g0, "Original graph");
    graph_print(g_exact, "Exact clique");
    graph_print(g_approx, "Approximated clique");

    utils_print_execution_time(time_exact, time_approx);

    /*graph_save_to_file(g0, "res/TEST_EXACT_CLIQUE_3.txt");
    graph_save_to_file(g_exact, "res/TEST_EXACT_CLIQUE_EXACT.txt");
    graph_save_to_file(g_approx, "res/TEST_EXACT_CLIQUE_APPROX.txt");*/

    printf("\nExact clique size: %d\n", graph_calc_clique_size(g_exact));
    printf("Approx clique size: %d\n", graph_calc_clique_size(g_approx));

    if (graph_clique_equal(g_exact, g_approx)) {
        print_test_pass(__func__);
        (*passed)++;
    }
    else {
        print_test_fail_msg(__func__, "approx");
        (*failed)++;
    }

    matrix_destroy(g0);
    matrix_destroy(g_exact);
    matrix_destroy(g_approx);

    return stats_create(time_exact, time_approx);
}

void test_clique_simple(int clique_size, int graph_size, int* passed, int* failed) {

    printf("\nTEST: %s - n: %d, clique size: %d\n", __func__, graph_size, clique_size);

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

    /* graph_save_to_file(g3, "res/TEST_EXACT_CLIQUE_3.txt");
     graph_save_to_file(g_exact, "res/TEST_EXACT_CLIQUE_EXACT.txt");
     graph_save_to_file(g_approx, "res/TEST_EXACT_CLIQUE_APPROX.txt");*/

    printf("\nExact clique size: %d\n", graph_calc_clique_size(g_exact));
    printf("Approx clique size: %d\n", graph_calc_clique_size(g_approx));

    if (graph_clique_equal(g_exact, g_approx)) {
        print_test_pass(__func__);
        (*passed)++;
    }
    else {
        print_test_fail_msg(__func__, "approx");
        (*failed)++;
    }

    matrix_destroy(g0);
    matrix_destroy(g2);
    matrix_destroy(g3);
    matrix_destroy(g_exact);
    matrix_destroy(g_approx);
}

void tests_clique(int* passed, int* failed) {

    printf("\n\nRunning clique tests...\n");

    test_clique_full_graph(passed, failed);
    PAUSE();

    test_clique_no_edges(passed, failed);
    PAUSE();

    // small graphs
    for (int i = 2; i <= 10; i += 2) {
            test_clique_random(i, passed, failed);
        PAUSE();
    }

    // medium graphs
    for (int i = 30; i <= 100; i += 10) {
        test_clique_random(i, passed, failed);
        PAUSE();
    }

    // large graphs - exact ~10s
    for (int i = 0; i < 1; i++) {
        test_clique_random(200, passed, failed);
        PAUSE();
    }

    // specified clique size
    test_clique_simple(50, 500, passed, failed);
}

void test_clique_from_args(matrix* g, int* passed, int* failed) {
    printf("\n\nRunning clique test...\n");

    matrix* g_exact = matrix_clone(g);

    clock_t time_exact = clock();
    exact_clique_bb_run(g_exact);
    time_exact = clock() - time_exact;

    clock_t time_approx = clock();
    matrix* g_approx = approx_clique_run(g);
    time_approx = clock() - time_approx;

    graph_print(g, "Original graph");
    graph_print(g_exact, "Exact clique");
    graph_print(g_approx, "Approximated clique");

    utils_print_execution_time(time_exact, time_approx);

    graph_save_to_file(g, "res/test_clique_graph.txt");
    graph_save_to_file(g_exact, "res/test_clique_exact.txt");
    graph_save_to_file(g_approx, "res/test_clique_approx.txt");

    if (graph_clique_equal(g_exact, g_approx)) {
        print_test_pass(__func__);
        (*passed)++;
    }
    else {
        print_test_fail_msg(__func__, "approx");
        (*failed)++;
    }

    printf("\nExact clique size: %d\n", graph_calc_clique_size(g_exact));
    printf("Approx clique size: %d\n", graph_calc_clique_size(g_approx));
}


void test_clique_stats(int* passed, int* failed) {
    FILE* stats_file = stats_clique_open_csv();

    for (int i = 200; i <= 600; i += 50) {
        stats* s_avg = stats_create(0, 0);
        for (int j = 0; j < TEST_SAMPLING; j++) {
            stats* s = test_clique_random(i, passed, failed);
            s_avg->exact_time += s->exact_time;
            s_avg->approx_time += s->approx_time;
            free(s);
        }

        s_avg->exact_time /= TEST_SAMPLING;
        s_avg->approx_time /= TEST_SAMPLING;

        stats_clique_save_to_file(stats_file, i, s_avg);

        free(s_avg);

        PAUSE();
    }

    fclose(stats_file);
}