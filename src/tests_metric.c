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

void test_metric_permuted_equal_graphs(int* passed, int* failed) {
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

void test_metric_noised_graphs(int* passed, int* failed) {
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

void test_metric_different_graphs_not_pass_condition1(int* passed, int* failed) {
    int size_g1 = 3;
    int size_g2 = 5;

    matrix* g1 = matrix_init(size_g1);
    graph_generate(g1, 7, 1, 1.0f, 1);  //clique

    matrix* g2 = matrix_init(size_g2);
    graph_generate(g2, 7, 1, 0.0f, 1);  //without edges

    float d = distance(g1, g2);

    graph_print(g1, "Graph 1");
    graph_print(g2, "Graph 2");
    printf("\nDistance: %f", d);

    if (d >= 14.875 && d <= 15.0) {
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

void test_metric_different_graphs_not_pass_condition2(int* passed, int* failed) {
    int size = 4;
    matrix* g1 = matrix_init(size);
    graph_generate(g1, 7, 1, 1.0f, 1);
    matrix* g2 = matrix_clone(g1);

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if(i!=j) g2->mat[j + i * size]++;

    float d = distance(g1, g2);

    graph_print(g1, "Graph 1");
    graph_print(g2, "Graph 2");
    printf("\nDistance: %f", d);

    if (d >= 0.75 && d <= 0.875) {
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

void test_metric_different_graphs_not_pass_condition3(int* passed, int* failed) {
    int size = 3;
    matrix* g1 = matrix_init(size);
    matrix* g2 = matrix_clone(g1);

    g1->mat[0 * size + 1] = 2;
    g1->mat[0 * size + 2] = 3;
    g1->mat[1 * size + 0] = 1;
    g1->mat[1 * size + 2] = 2;
    g1->mat[2 * size + 0] = 3;
    g1->mat[2 * size + 1] = 3;
  
    g2->mat[0 * size + 1] = 2;
    g2->mat[0 * size + 2] = 1;
    g2->mat[1 * size + 0] = 2;
    g2->mat[1 * size + 2] = 4;
    g2->mat[2 * size + 0] = 1;
    g2->mat[2 * size + 1] = 4;

    float d = distance(g1, g2);

    graph_print(g1, "Graph 1");
    graph_print(g2, "Graph 2");
    printf("\nDistance: %f", d);

    if (d >= 0.625 && d <= 0.75) {
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

void test_metric_different_graphs_not_pass_condition4(int* passed, int* failed) {
    int size = 6;
    matrix* g1 = matrix_init(size);
    matrix* g2 = matrix_clone(g1);

    g1->mat[0* size + 1] = 1;
    g1->mat[1* size + 0] = 1;
    g1->mat[1* size + 2] = 1;
    g1->mat[2* size + 1] = 1;
    g1->mat[2* size + 3] = 1;
    g1->mat[2* size + 4] = 1;
    g1->mat[3* size + 2] = 1;
    g1->mat[4* size + 2] = 1;
    g1->mat[4* size + 5] = 1;
    g1->mat[5* size + 4] = 1;

    g2->mat[0 * size + 1] = 1;
    g2->mat[1 * size + 0] = 1;
    g2->mat[1 * size + 2] = 1;
    g2->mat[2 * size + 1] = 1;
    g2->mat[2 * size + 3] = 1;
    g2->mat[3 * size + 2] = 1;
    g2->mat[3 * size + 4] = 1;
    g2->mat[3 * size + 5] = 1;
    g2->mat[4 * size + 3] = 1;
    g2->mat[5 * size + 3] = 1;

    float d = distance(g1, g2);

    graph_print(g1, "Graph 1");
    graph_print(g2, "Graph 2");
    printf("\nDistance: %f", d);

    if (d >= 0.5 && d <= 0.675) {
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

void test_metric_different_graphs_pass_condition4(int* passed, int* failed) {
    int size = 10;
    matrix* g1 = matrix_init(size);
    matrix* g2 = matrix_clone(g1);

    g1->mat[0 * size + 1] = 1;
    g1->mat[1 * size + 0] = 1;
    g1->mat[1 * size + 2] = 1;
    g1->mat[2 * size + 1] = 1;
    g1->mat[1 * size + 3] = 1;
    g1->mat[3 * size + 1] = 1;
    g1->mat[2 * size + 4] = 1;
    g1->mat[4 * size + 2] = 1;
    g1->mat[3 * size + 5] = 1;
    g1->mat[5 * size + 3] = 1;
    g1->mat[5 * size + 7] = 1;
    g1->mat[7 * size + 5] = 1;
    g1->mat[4 * size + 6] = 1;
    g1->mat[6 * size + 4] = 1;
    g1->mat[6 * size + 8] = 1;
    g1->mat[8 * size + 6] = 1;
    g1->mat[7 * size + 8] = 1;
    g1->mat[8 * size + 7] = 1;
    g1->mat[8 * size + 9] = 1;
    g1->mat[9 * size + 8] = 1;

    g2->mat[0 * size + 1] = 1;
    g2->mat[1 * size + 0] = 1;
    g2->mat[1 * size + 2] = 1;
    g2->mat[2 * size + 1] = 1;
    g2->mat[1 * size + 3] = 1;
    g2->mat[3 * size + 1] = 1;
    g2->mat[2 * size + 4] = 1;
    g2->mat[4 * size + 2] = 1;
    g2->mat[3 * size + 5] = 1;
    g2->mat[5 * size + 3] = 1;
    g2->mat[5 * size + 7] = 1;
    g2->mat[7 * size + 5] = 1;
    g2->mat[4 * size + 6] = 1;
    g2->mat[6 * size + 4] = 1;
    g2->mat[6 * size + 8] = 1;
    g2->mat[8 * size + 6] = 1;
    g2->mat[7 * size + 8] = 1;
    g2->mat[8 * size + 7] = 1;
    g2->mat[7 * size + 9] = 1;
    g2->mat[9 * size + 7] = 1;

    float d = distance(g1, g2);

    graph_print(g1, "Graph 1");
    graph_print(g2, "Graph 2");
    printf("\nDistance: %f", d);

    if (d == 0.0) {
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
    
    test_metric_permuted_equal_graphs(passed, failed);
    PAUSE();

    test_metric_noised_graphs(passed, failed);
    PAUSE();

    test_metric_different_graphs_not_pass_condition1(passed, failed);
    PAUSE();

    test_metric_different_graphs_not_pass_condition2(passed, failed);
    PAUSE();

    test_metric_different_graphs_not_pass_condition3(passed, failed);
    PAUSE();

    test_metric_different_graphs_not_pass_condition4(passed, failed);
    PAUSE();

    test_metric_different_graphs_pass_condition4(passed, failed);
    PAUSE();
}

void test_metric_from_args(matrix* g1, matrix* g2, int* passed, int* failed) {
    printf("\n\nRunning metrics test...\n");

    float d = distance(g1, g2);

    graph_print(g1, "Graph 1");
    graph_print(g2, "Graph 2");

    printf("\nDistance: %f\n", d);

    (*passed)++;
    print_test_pass(__func__);
}
