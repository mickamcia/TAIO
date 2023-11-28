#include "tests.h"

void test_metric_equal_graphs(int* passed, int* failed) {
    int size = 15;
    matrix* g1 = matrix_init(size);
    graph_generate(g1, 7, 1, 0.8f, 1);
    matrix* g2 = matrix_clone(g1);

    float d = distance(g1, g2);

    graph_print(g1, "Graph 1");
    graph_print(g2, "Graph 2");
    printf("\nDistance: %f", d);

    if (d == 0) {
        print_test_pass(__func__);
        (*passed)++;
    }
    else {
        print_test_fail(__func__);
        (*failed)++;
    }

    matrix_destroy(g1);
    matrix_destroy(g2);
}

void test_metric_permuted_graphs(int* passed, int* failed) {
    int size = 20;
    matrix* g1 = matrix_init(size);
    graph_generate(g1, 1, 0, 0.8f, 1);
    matrix* g2 = matrix_clone(g1);
    graph_permute(g2);

    graph_print(g1, "Graph 1");
    graph_print(g2, "Graph 2");

    float d = distance(g1, g2);

    printf("\nDistance: %f", d);

    if (d == 0) {
        print_test_pass(__func__);
        (*passed)++;
    }
    else {
        print_test_fail(__func__);
        (*failed)++;
    }

    matrix_destroy(g1);
    matrix_destroy(g2);
}

void test_metric_different_graphs(int* passed, int* failed) {
    int size = 11;
    matrix* g1 = matrix_init(size);
    graph_generate(g1, 7, 1, 0.8f, 1);
    matrix* g2 = matrix_clone(g1);

    graph_add_noise(g2, 0.5f, 1, 0.0);

    float d = distance(g1, g2);

    graph_print(g1, "Graph 1");
    graph_print(g2, "Graph 2");
    printf("\nDistance: %f", d);

    if (d != 0) {
        print_test_pass(__func__);
        (*passed)++;
    }
    else {
        print_test_fail(__func__);
        (*failed)++;
    }

    matrix_destroy(g1);
    matrix_destroy(g2);
}


void tests_metric(int* passed, int* failed) {

    printf("Running metrics tests...\n");

    test_metric_equal_graphs(passed, failed);
    PAUSE();
    
    test_metric_permuted_graphs(passed, failed);
    PAUSE();

    test_metric_different_graphs(passed, failed);
    PAUSE();
}

void test_metric_from_args(matrix* g1, matrix* g2, int* passed, int* failed) {
    printf("\n\nRunning metrics test...\n");

    float d = distance(g1, g2);

    graph_print(g1, "Graph 1");
    graph_print(g2, "Graph 2");

    printf("\nDistance: %f\n", d);

    (*passed)++;
    //print_test_pass(__func__);
}
