#include <stdlib.h>
#include <limits.h>
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

void test_clique_full_graph(int* passed, int* failed, int* approx_failed) {
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
    matrix* g_approx = approx_clique_run(g3);
    time_approx = clock() - time_approx;

    //graph_print(g3);
    graph_print(g_exact, "Exact clique");
    graph_print(g_approx, "Approximated clique");

    utils_print_execution_time(time_exact, time_approx);

    if (GENERATE_EXAMPLES)
        graph_save_to_file(g3, "examples/example_clique_1.txt");

    printf("\nExact clique size: %d\n", graph_calc_clique_size(g_exact));
    printf("Approx clique size: %d\n", graph_calc_clique_size(g_approx));

    if (graph_clique_equal(g_exact, g_approx)) {
        print_test_pass(__func__);
        (*passed)++;
    }
    else {
        print_test_approx_fail(__func__);
        (*approx_failed)++;
    }

    matrix_destroy(g0);
    matrix_destroy(g2);
    matrix_destroy(g3);
    matrix_destroy(g_exact);
    matrix_destroy(g_approx);
}

void test_clique_no_edges(int* passed, int* failed, int * approx_failed) {
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
    matrix* g_approx = approx_clique_run(g3);
    time_approx = clock() - time_approx;

    //graph_print(g3);
    graph_print(g_exact, "Exact clique");
    graph_print(g_approx, "Approximated clique");

    utils_print_execution_time(time_exact, time_approx);

    if (GENERATE_EXAMPLES)
        graph_save_to_file(g3, "examples/example_clique_0.txt");

    printf("\nExact clique size: %d\n", graph_calc_clique_size(g_exact));
    printf("Approx clique size: %d\n", graph_calc_clique_size(g_approx));

    if (graph_clique_equal(g_exact, g_approx)) {
        print_test_pass(__func__);
        (*passed)++;
    }
    else {
        print_test_approx_fail(__func__);
        (*approx_failed)++;
    }

    matrix_destroy(g0);
    matrix_destroy(g2);
    matrix_destroy(g3);
    matrix_destroy(g_exact);
    matrix_destroy(g_approx);
}

stats* test_clique_random(int graph_size, int* passed, int* failed, int* approx_failed) {
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

    printf("\nExact clique size: %d\n", graph_calc_clique_size(g_exact));
    printf("Approx clique size: %d\n", graph_calc_clique_size(g_approx));

    if (graph_clique_equal(g_exact, g_approx)) {
        print_test_pass(__func__);
        (*passed)++;
    }
    else {
        print_test_approx_fail(__func__);
        (*approx_failed)++;
    }

    if (GENERATE_EXAMPLES) {
        char file_name[50];
        sprintf(file_name, "examples/example_clique_%d.txt", graph_size);
        graph_save_to_file(g0, file_name);
    }

    matrix_destroy(g0);
    matrix_destroy(g_exact);
    matrix_destroy(g_approx);

    return stats_create(time_exact, time_approx);
}

void test_clique_simple(int clique_size, int graph_size, int* passed, int* failed, int* approx_failed) {

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
    matrix* g_approx = approx_clique_run(g3);
    time_approx = clock() - time_approx;

    //graph_print(g3);
    graph_print(g_exact, "Exact clique");
    graph_print(g_approx, "Approximated clique");

    utils_print_execution_time(time_exact, time_approx);

    if (GENERATE_EXAMPLES) {
        char f1[50];
        sprintf(f1, "examples/example_clique_%d.txt", *passed + *failed + *approx_failed);
        graph_save_to_file(g3, f1);
    }

    printf("\nExact clique size: %d\n", graph_calc_clique_size(g_exact));
    printf("Approx clique size: %d\n", graph_calc_clique_size(g_approx));

    if (graph_clique_equal(g_exact, g_approx)) {
        print_test_pass(__func__);
        (*passed)++;
    }
    else {
        print_test_approx_fail(__func__);
        (*approx_failed)++;
    }

    matrix_destroy(g0);
    matrix_destroy(g2);
    matrix_destroy(g3);
    matrix_destroy(g_exact);
    matrix_destroy(g_approx);
}

void tests_clique(int* passed, int* failed, int* approx_failed) {

    printf("\n\nRunning clique tests...\n");

    test_clique_full_graph(passed, failed, approx_failed);
    PAUSE();

    test_clique_no_edges(passed, failed, approx_failed);
    PAUSE();

    // small graphs
    for (int i = 2; i <= 10; i += 2) {
        stats* s = test_clique_random(i, passed, failed, approx_failed);
        free(s);
        PAUSE();
    }

    // medium graphs
    for (int i = 30; i <= 100; i += 10) {
        stats* s = test_clique_random(i, passed, failed, approx_failed);
        free(s);
        PAUSE();
    }

    // large graphs - exact ~10s
    for (int i = 0; i < 1; i++) {
        stats* s = test_clique_random(200, passed, failed, approx_failed);
        free(s);
        PAUSE();
    }

    // specified clique size
    test_clique_simple(50, 500, passed, failed, approx_failed);
}

void test_clique_from_args(matrix* g, bool run_exact, bool run_approx, int* passed, int* failed, int* approx_failed) {
    printf("\n\nRunning clique test...\n");

    matrix* g_exact = matrix_clone(g);

    clock_t time_exact;
    if (run_exact) {
        time_exact = clock();
        exact_clique_bb_run(g_exact);
        time_exact = clock() - time_exact;
    }

    clock_t time_approx;
    matrix* g_approx = NULL;
    if (run_approx) {
        time_approx = clock();
        g_approx = approx_clique_run(g);
        time_approx = clock() - time_approx;
    }

    graph_print(g, "Original graph");

    if (run_exact)
        graph_print(g_exact, "Exact clique");

    if (run_approx)
        graph_print(g_approx, "Approximated clique");


    //graph_save_to_file(g, "test_clique_graph.txt");
    if (run_exact)
        graph_save_to_file(g_exact, "test_clique_exact.txt");
    if (run_approx)
        graph_save_to_file(g_approx, "test_clique_approx.txt");

    if (run_exact && run_approx) {
        utils_print_execution_time(time_exact, time_approx);
        if (graph_clique_equal(g_exact, g_approx)) {
            print_test_pass(__func__);
            (*passed)++;
        }
        else {
            print_test_approx_fail(__func__);
            (*approx_failed)++;
        }
    }

    if (run_exact)
        printf("\nExact clique size: %d\n", graph_calc_clique_size(g_exact));
    if (run_approx)
        printf("Approx clique size: %d\n", graph_calc_clique_size(g_approx));
}


void test_clique_stats(int* passed, int* failed, int* approx_failed) {
    FILE* stats_file = stats_clique_open_csv();

    for (int n = 200; n <= 600; n += 50) {
        stats* s_avg = stats_create(0, 0);
        stats* sample_min_val = stats_create(INT_MAX, INT_MAX);
        stats* sample_max_val = stats_create(0, 0);

        for (int i = 0; i < TEST_SAMPLING; i++) {
            printf("\nSample %d/%d:", i + 1, TEST_SAMPLING);
            stats* s = test_clique_random(n, passed, failed, approx_failed);
            s_avg->exact_time += s->exact_time;
            s_avg->approx_time += s->approx_time;

            sample_min_val->exact_time = minimum(sample_min_val->exact_time, s->exact_time);
            sample_min_val->approx_time = minimum(sample_min_val->approx_time, s->approx_time);
            sample_max_val->exact_time = maximum(sample_max_val->exact_time, s->exact_time);
            sample_max_val->approx_time = maximum(sample_max_val->approx_time, s->approx_time);

            free(s);
        }

        s_avg->exact_time -= sample_min_val->exact_time;
        s_avg->exact_time -= sample_max_val->exact_time;

        s_avg->approx_time -= sample_min_val->approx_time;
        s_avg->approx_time -= sample_max_val->approx_time;

        s_avg->exact_time /= (TEST_SAMPLING - 2);
        s_avg->approx_time /= (TEST_SAMPLING - 2);

        stats_clique_save_to_file(stats_file, n, s_avg);

        free(s_avg);

        PAUSE();
    }

    fclose(stats_file);
}