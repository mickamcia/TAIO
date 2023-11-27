#include <stdlib.h>
#include <string.h>
#include "metric.h"
#include "utils.h"
#include "graph.h"
#include "limits.h"

// internal
int* build_degree_vector(matrix* g, int* newSize);
int vector_diff(int* v1, int* v2, int size);
int get_graph_n(matrix* g);
int get_graph_m_connected(matrix* g);
int sum_acc(int g1_n, int g2_n);
float err(matrix* g1, matrix* g2, int n, int m);

int distance(matrix* g1, matrix* g2) {

	int g1_n, g1_m, g2_n, g2_m;
	int sum_acc_value;
	float err_value = 0; 

	g1_n = get_graph_n(g1);
	g2_n = get_graph_n(g2);
	g1_m = get_graph_m_connected(g1);
	g2_m = get_graph_m_connected(g2);
	
	//printf("\ng1_n: %d, g2_n: %d, g1_m: %d, g2_m: %d\n", g1_n, g2_n, g1_m, g2_m);
	
	if (g1_n >= g2_n) {
	
		sum_acc_value = sum_acc(g2_n, g1_n);
	}
	else 
	{
		sum_acc_value = sum_acc(g1_n, g2_n);
	}
	
	if (g1_n == g2_n && g1_m == g2_m)
	{
		err_value = err(g1, g2, g1_n,g1_m);
	}

	return  sum_acc_value + abs(g1_n - g2_n) + abs(g1_m - g2_m) + err_value;
}


int get_graph_n(matrix* g) 
{
	int size = 0;
	for (int i = 0; i < g->size; i++)
		if (g->mat[i * g->size + i] == 0)
			size++;
	return size;
}

int get_graph_m_connected(matrix* g) 
{
	int m = 0;
	for (int i = 0; i < g->size; i++) {
		if (g->mat[i * g->size + i] == 0) {
			for (int j = 0; j < g->size; j++) {
				if (g->mat[i * g->size + j] > 0) {
					m++;
				}
			}
		}
	}
	return m;
}

int sum_acc(int g1_n, int g2_n)
{
	int result = 0;
	for (int i = g1_n; i < g2_n; i++)
		result += (i * (i - 1));
	return result;
}

float err(matrix* g1, matrix* g2, int n, int m) {
	
	float err_outgoing_edges_acc_diff;
	float err_outgoing_edges_diff;
	float err_neighbour_outgoing_edges_acc_diff;
	float err_neighbour_outgoing_edges_diff;
	


	//int* v1 = build_degree_vector(g1, &v1_size);
	//int* v2 = build_degree_vector(g2, &v2_size);

	//if (v1_size != v2_size)
	//	return INT_MAX;

	//int d = vector_diff(v1, v2, v1_size);

	//free(v1);
	//free(v2);

	return 0;
}

int* build_degree_vector(matrix* g, int* newSize) {
	*newSize = 0;
	for (int i = 0; i < g->size; i++)
		if (g->mat[i * g->size + i] == 0)
			(*newSize)++;

	int* vector = (int*)malloc(sizeof(int) * *newSize);
	if (!vector)
		ERR("malloc");

	memset(vector, 0, sizeof(int) * *newSize);

	int v_i = 0, v_j = 0;
	for (int i = 0; i < g->size; i++) {
		if (g->mat[i * g->size + i] == 0) {
			for (int j = 0; j < g->size; j++) {
				if (g->mat[i * g->size + j] > 0) {
					vector[v_i] += g->mat[i * g->size + j];
				}
			}
			v_i++;
		}
	}

	bubble_sort(vector, *newSize);
	return vector;
}

int vector_diff(int* v1, int* v2, int size) {
	int diff = 0;

	for (int i = 0; i < size; i++) {
		diff += abs(v1[i] - v2[i]);
	}

	return diff;
}