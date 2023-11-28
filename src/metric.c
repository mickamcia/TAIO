#include <stdlib.h>
#include <string.h>
#include "metric.h"
#include "utils.h"
#include "graph.h"
#include "limits.h"
#define SCALE_TAB 5

// internal
int* build_degree_self_matrix(matrix* g, int size);
int* build_degree_neighbour_matrix(matrix* g, int* self, int size);
float count_err_acc_diff(int* m1, int* m2, int size);
float count_err_diff(int* m1, int* m2, int size);
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

	float scales[SCALE_TAB] = {1/2.0 , 1 / 2.0 + 1 / 8.0, 1-1/4.0 ,1 - 1 / 8.0, 1};
	float err_outgoing_edges_acc_diff;
	float err_outgoing_edges_diff;
	float err_neighbour_outgoing_edges_acc_diff;
	int* m1_self, *m2_self, *m1_neighbour, *m2_neighbour;

	if(g1_n< g2_n)
	{
		return scale(scales[3], scales[4], (g2_n- g1_n)/(float)g2_n);
	}
	else if (g1_n > g2_n) {
		return scale(scales[3], scales[4], (g1_n - g2_n) / (float)g1_n);
	}

	m1_self = build_degree_self_matrix(g1, g1_n);
	m2_self = build_degree_self_matrix(g2, g2_n);
	array2d_print(m1_self, g1_n, g1_n, "g1");
	array2d_print(m2_self, g2_n, g2_n, "g2");

	m1_neighbour = build_degree_neighbour_matrix(g1, m1_self, g1_n);
	m2_neighbour = build_degree_neighbour_matrix(g2, m2_self, g2_n);

	array2d_print(m1_neighbour, g1_n, g1_n, "neig g1");
	array2d_print(m2_neighbour, g2_n, g2_n, "neig g2");

	sort_degreeMatrix(m1_self, g1_n);
	sort_degreeMatrix(m2_self, g2_n);

	array2d_print(m1_self, g1_n, g1_n, "sorted g1");
	array2d_print(m2_self, g2_n ,g2_n ,"sorted g2");

	err_outgoing_edges_acc_diff = count_err_acc_diff(m1_self, m2_self, g1_n);
	printf("%f\n", err_outgoing_edges_acc_diff);

	if (err_outgoing_edges_acc_diff != 0)
	{
		return scale(scales[2], scales[3], err_outgoing_edges_acc_diff);
	}

	err_outgoing_edges_diff = count_err_diff(m1_self, m2_self, g1_n);
	printf("%f\n", err_outgoing_edges_diff);

	if (err_outgoing_edges_diff != 0)
	{
		return scale(scales[1], scales[2], err_outgoing_edges_diff);
	}

	free(m1_self);
	free(m2_self);

	sort_degreeMatrix(m1_neighbour, g1_n);
	sort_degreeMatrix(m2_neighbour, g2_n);

	array2d_print(m1_neighbour, g1_n, g1_n, "g1");
	array2d_print(m2_neighbour, g2_n, g2_n, "g2");

	err_neighbour_outgoing_edges_acc_diff = count_err_diff(m1_neighbour, m2_neighbour, g1_n);
	printf("%f\n", err_neighbour_outgoing_edges_acc_diff);

	if (err_neighbour_outgoing_edges_acc_diff != 0)
	{
		return scale(scales[0], scales[1], err_neighbour_outgoing_edges_acc_diff);
	}

	free(m1_neighbour);
	free(m2_neighbour);

	return 0;
}

float scale(float lower, float upper, float value)
{
	printf("%f,%f,%f\n", lower, upper, value);
	return lower + (upper - lower) * value;
}

int* build_degree_self_matrix(matrix* g, int size) {

	int* deg_mat = (int*)malloc(sizeof(int) * size * size);
	if (!deg_mat)
		ERR("malloc");
	memset(deg_mat, 0, sizeof(int) * size * size);

	int v_i = 0;
	for (int i = 0; i < g->size; i++) {
		if (g->mat[i * g->size + i] == 0) {
			int v_j = 0;
			for (int j = 0; j < g->size; j++) {
				if (g->mat[i * g->size + j] > 0) {
					deg_mat[size * v_i + (size-1)] += g->mat[i * g->size + j];
					deg_mat[size* v_i + v_j] = g->mat[i * g->size + j];
					++v_j;
				}
			}
		++v_i;
		}
	}
	return deg_mat;
}

int* build_degree_neighbour_matrix(matrix* g, int* self, int size) {

	int* deg_mat = (int*)malloc(sizeof(int) * size * size);
	if (!deg_mat)
		ERR("malloc");
	memset(deg_mat, 0, sizeof(int) * size * size);

	int v_i = 0;
	for (int i = 0; i < g->size; i++) {
		if (g->mat[i * g->size + i] == 0) {
			int v_j = 0;
			int v_index = 0;
			for (int j = 0; j < g->size; j++) {
				if (g->mat[i * g->size + j] > 0) {
					deg_mat[size * v_i + (size - 1)] += self[v_index * size + (size - 1)];
					deg_mat[size * v_i + v_j] = self[v_index * size + (size - 1)];
					++v_j;
				}
				if (g->mat[i * g->size + j] != -1) v_index++;
			}
			++v_i;
		}
	}
	return deg_mat;
}

float count_err_acc_diff(int* m1, int* m2, int size) {
	float diff = 0;
	float max_sum = 0;
	for (int i = 0; i < size; i++) {
		diff += abs(m1[(size-1) + i*size] - m2[(size - 1) + i * size]);
		max_sum += max(m1[(size - 1) + i * size], m2[(size - 1) + i * size]);
	}
	printf("%f, %f\n", diff, max_sum);
	return diff / max_sum;
}

float count_err_diff(int* m1, int* m2, int size) {
	float diff = 0;
	float max_sum = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size-1; j++) {
		diff += abs(m1[j + i * size] - m2[j + i * size]);
		max_sum += max(m1[j + i * size],m2[j + i * size]);
		}
	}
	printf("%f, %f\n", diff, max_sum);
	return diff / max_sum;
}