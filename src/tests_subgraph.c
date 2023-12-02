#include <stdlib.h>
#include <limits.h>
#include "tests.h"
#include "stats.h"

void test_exact_subgraph_simple(int* passed, int* failed) {
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

    graph_print(g0, "Subgraph");
    graph_print(g1, "Graph 1");
    graph_print(g2, "Subgraph from G1");
    graph_print(g3, "Subgraph from G2");

    //graph_save_to_file(g0, "res/TEST_EXACT_SUBGRAPH_SIMPLE_0.txt");
    //graph_save_to_file(g1, "res/TEST_EXACT_SUBGRAPH_SIMPLE_1.txt");
    //graph_save_to_file(g2, "res/TEST_EXACT_SUBGRAPH_SIMPLE_2.txt");
    //graph_save_to_file(g3, "res/TEST_EXACT_SUBGRAPH_SIMPLE_3.txt");

    matrix_destroy(g0);
    matrix_destroy(g1);
    matrix_destroy(g2);
    matrix_destroy(g3);
}

void test_exact_subgraph(int* passed, int* failed) {
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

    graph_print(g0, "Subgraph");
    graph_print(g5, "Graph A");
    graph_print(g7, "Subgraph from A");
    graph_print(g6, "Graph B");
    graph_print(g8, "Subgraph from B");

    //graph_save_to_file(g0, "res/TEST_EXACT_SUBGRAPH_5.txt");
    //graph_save_to_file(g5, "res/TEST_EXACT_SUBGRAPH_5.txt");
    //graph_save_to_file(g6, "res/TEST_EXACT_SUBGRAPH_6.txt");
    //graph_save_to_file(g7, "res/TEST_EXACT_SUBGRAPH_7.txt");
    //graph_save_to_file(g8, "res/TEST_EXACT_SUBGRAPH_8.txt");

    matrix_destroy(g0);
    matrix_destroy(g3);
    matrix_destroy(g4);
    matrix_destroy(g5);
    matrix_destroy(g6);
    matrix_destroy(g7);
    matrix_destroy(g8);
}

void test_approx_subgraph(int* passed, int* failed) {
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

    graph_print(g5, "Graph A");
    graph_print(g7, "Subgraph from A");
    graph_print(g6, "Graph B");
    graph_print(g8, "Subgraph from B");

    //graph_save_to_file(g5, "res/TEST_APPROX_SUBGRAPH_5.txt");
    //graph_save_to_file(g6, "res/TEST_APPROX_SUBGRAPH_6.txt");
    //graph_save_to_file(g7, "res/TEST_APPROX_SUBGRAPH_7.txt");
    //graph_save_to_file(g7, "res/TEST_APPROX_SUBGRAPH_8.txt");

    matrix_destroy(g0);
    matrix_destroy(g3);
    matrix_destroy(g4);
    matrix_destroy(g5);
    matrix_destroy(g6);
    matrix_destroy(g7);
    matrix_destroy(g8);

}

stats* test_subgraph(int subgraph_size, int graph_a_size, int graph_b_size, int* passed, int* failed, int* approx_failed) {
    printf("\nTEST: %s - subgraph size: %d, A size: %d, B size: %d\n", __func__, subgraph_size, graph_a_size, graph_b_size);

    //const int subgraph_size = 5;
    //const int graph_a_size = 12;
    //const int graph_b_size = 9;

    matrix* g0 = matrix_init(subgraph_size);
    graph_generate(g0, 7, 1, 0.6f, 1);

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

    graph_print(g0, "Subgraph");
    graph_print(g5, "Original A");
    graph_print(a_exact, "Exact subgraph of A");
    graph_print(a_approx, "Approx subgraph of A");
    graph_print(g6, "Original B");
    graph_print(b_exact, "Exact subgraph of B");
    graph_print(b_approx, "Approx subgraph of B");

    utils_print_execution_time(time_exact, time_approx);

    if (distance(g0, a_exact) == 0 &&
        distance(g0, b_exact) == 0)
    {

        if (
            distance(a_exact, a_approx) == 0 &&
            distance(b_exact, b_approx) == 0
            )
        {
            (*passed)++;
            print_test_pass(__func__);
        }
        else {
            (*approx_failed)++;
            print_test_approx_fail(__func__);
        }
    }
    else
    {
        print_test_fail(__func__);
        (*failed)++;
    }

    if (GENERATE_EXAMPLES) {
        char f1[50];
        char f2[50];
        sprintf(f1, "examples/example_subgraph_%d_g1.txt", *passed + *failed + *approx_failed);
        sprintf(f2, "examples/example_subgraph_%d_g2.txt", *passed + *failed + *approx_failed);
        graph_save_to_file(g5, f1);
        graph_save_to_file(g6, f2);
    }

    matrix_destroy(g0);
    matrix_destroy(g3);
    matrix_destroy(g4);
    matrix_destroy(g5);
    matrix_destroy(g6);
    matrix_destroy(a_exact);
    matrix_destroy(a_approx);
    matrix_destroy(b_exact);
    matrix_destroy(b_approx);

    return stats_create(time_exact, time_approx);
}

void tests_subgraph(int* passed, int* failed, int* approx_failed) {
    stats* s;
    printf("\n\nRunning subgraphs tests...\n");
    
    s = test_subgraph(3, 4, 5, passed, failed, approx_failed);
    PAUSE();
    free(s);

    s = test_subgraph(5, 5, 5, passed, failed, approx_failed);
    PAUSE();
    free(s);

    s = test_subgraph(8, 15, 18, passed, failed, approx_failed);
    PAUSE();
    free(s);

    s = test_subgraph(10, 16, 17, passed, failed, approx_failed);
    PAUSE();
    free(s);

    s = test_subgraph(10, 18, 20, passed, failed, approx_failed);
    PAUSE();
    free(s);
}

void test_subgraph_from_args(matrix* g1, matrix* g2, int* passed, int* failed, int* approx_failed) {
    printf("\n\nRunning subgraphs test...\n");
    matrix* a_exact = matrix_clone(g1);
    matrix* b_exact = matrix_clone(g2);

    clock_t time_exact = clock();
    exact_subgraph_run(a_exact, b_exact);
    time_exact = clock() - time_exact;

    matrix* a_approx = matrix_clone(g1);
    matrix* b_approx = matrix_clone(g2);

    clock_t time_approx = clock();
    approx_subgraph_run(a_approx, b_approx);
    time_approx = clock() - time_approx;

    graph_print(g1, "Original A");
    graph_print(g2, "Original B");
    graph_print(a_exact, "Exact subgraph of A");
    graph_print(b_exact, "Exact subgraph of B");
    graph_print(a_approx, "Approx subgraph of A");
    graph_print(b_approx, "Approx subgraph of B");

    utils_print_execution_time(time_exact, time_approx);

    if (
        distance(a_exact, a_approx) == 0 &&
        distance(b_exact, b_approx) == 0
        )
    {
        print_test_pass(__func__);
        (*passed)++;
    }
    else {
        print_test_approx_fail(__func__);
        (*approx_failed)++;
    }

    //graph_save_to_file(g1, "test_subgraph_a.txt");
    graph_save_to_file(a_exact, "test_subgraph_a_exact.txt");
    graph_save_to_file(a_approx, "test_subgraph_a_approx.txt");
    //graph_save_to_file(g2, "test_subgraph_b.txt");
    graph_save_to_file(b_exact, "test_subgraph_b_exact.txt");
    graph_save_to_file(b_approx, "test_subgraph_b_approx.txt");
}

void test_subgraph_stats(int* passed, int* failed, int* approx_failed) {
    FILE* stats_file = stats_subgraph_open_csv();

    for (int n1 = 5; n1 < 22; n1 += 3) {
        int n2 = (double)n1 * 1.25;

        stats* s_avg = stats_create(0, 0);
        stats* sample_min_val = stats_create(INT_MAX, INT_MAX);
        stats* sample_max_val = stats_create(0, 0);

        int subgraph_size = minimum(10, n1 - 1);

        for (int i = 0; i < TEST_SAMPLING; i++) {
            printf("\nSample %d/%d:", i + 1, TEST_SAMPLING);
            stats* s = test_subgraph(subgraph_size, n1, n2, passed, failed, approx_failed);
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

        stats_subgraph_save_to_file(stats_file, n1, n2, s_avg);

        free(s_avg);
    }

    fclose(stats_file);
}