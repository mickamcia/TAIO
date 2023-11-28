#include <stdlib.h>
#include <string.h>
#include "metric.h"
#include "utils.h"
#include "graph.h"
#include "limits.h"
#define SCALE_TAB 6

// internal
int* build_degree_vector(matrix* g, int size);
float vector_diff(int* v1, int* v2, int size);
int get_graph_n(matrix* g);
int get_graph_m_connected(matrix* g);
int sum_acc(int g1_n, int g2_n);
float err(matrix* g1, matrix* g2, int  g1_n, int g1_m, int g2_n,int  g2_m);
float scale(float lower, float upper, float value);

float distance(matrix* g1, matrix* g2) {

	int g1_n, g1_m, g2_n, g2_m;
	int sum_acc_value = 0;
	int m_diff_value;

	g1_n = get_graph_n(g1);
	g2_n = get_graph_n(g2);
	g1_m = get_graph_m_connected(g1);
	g2_m = get_graph_m_connected(g2);
	
	printf("\ng1_n: %d, g2_n: %d, g1_m: %d, g2_m: %d\n", g1_n, g2_n, g1_m, g2_m);
	
	if (g1_n > g2_n) {
	
		sum_acc_value = sum_acc(g2_n, g1_n);
		m_diff_value = g1_m - g2_m;
	}
	else if (g1_n == g2_n) 
	{
		m_diff_value = abs(g1_m - g2_m);
	}
	else 
	{
		sum_acc_value = sum_acc(g1_n, g2_n);
		m_diff_value = g2_m - g1_m;
	}

	return  sum_acc_value + m_diff_value + abs(g1_n - g2_n) + err(g1, g2, g1_n, g1_m, g2_n, g2_m);
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


float err(matrix* g1, matrix* g2, int  g1_n, int g1_m, int g2_n, int  g2_m){

	float scales[SCALE_TAB] = {1/2.0, 1/2.0 + 1/16.0 ,1 / 2.0 + 1 / 8.0 ,1-1/4.0,1 - 1 / 8.0,1};
	float err_outgoing_edges_acc_diff;
	float err_outgoing_edges_diff;
	float err_neighbour_outgoing_edges_acc_diff;
	float err_neighbour_outgoing_edges_diff;
	int* v1; 
	int* v2; 

	if(g1_n< g2_n)
	{
		return scale(scales[4], scales[5], g1_n/(float)g2_n);
	}
	else if (g1_n > g2_n) {
		return scale(scales[4], scales[5], g2_n / (float)g1_n);
	}

	v1 = build_degree_vector(g1, g1_n);
	v2 = build_degree_vector(g2, g1_n);
	list_print(v1, g1_n);
	list_print(v2, g1_n);

	err_outgoing_edges_acc_diff = vector_diff(v1, v2, g1_n);
	printf("%f\n", err_outgoing_edges_acc_diff);

	if (err_outgoing_edges_acc_diff != 0)
	{
		free(v1);
		free(v2);
		return scale(scales[3], scales[4], err_outgoing_edges_acc_diff);
	}



	return 0;
}

float scale(float lower, float upper, float value)
{
	printf("%f,%f,%f\n", lower, upper, value);
	return lower + (upper - lower) * value;
}

int* build_degree_vector(matrix* g, int size) {

	int* vector = (int*)malloc(sizeof(int) * size);
	if (!vector)
		ERR("malloc");

	memset(vector, 0, sizeof(int) * size);

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

	bubble_sort(vector, size);
	return vector;
}

float vector_diff(int* v1, int* v2, int size) {
	float diff = 0;
	float sum = 0;
	for (int i = 0; i < size; i++) {
		diff += abs(v1[i] - v2[i]);
		sum += (v1[i] + v2[i]);
	}
	printf("%f, %f\n", diff, sum);
	return diff/sum;
}