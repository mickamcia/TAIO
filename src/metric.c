#include <stdlib.h>
#include <string.h>
#include "metric.h"
#include "utils.h"
#include "graph.h"
#include "limits.h"
#define SCALE_TAB 5

// internal
int graph_get_n(matrix* g);
int graph_get_m_unique(matrix* g);
int multigraph_position_sum(int g1_n, int g2_n);
float multigraph_similarity(matrix* g1, matrix* g2, int  g1_n, int g2_n);
float scale(float lower, float upper, float value);
int* build_degree_self_matrix(matrix* g, int size);
int* build_degree_neighbour_matrix(matrix* g, int* self, int size);
float count_err_acc_diff(int* m1, int* m2, int size);
float count_err_diff(int* m1, int* m2, int size);

void sort_degreeMatrix(int* arr, int size);
void sort_each_vector_in_degreeMatrix(int* arr, int size);
void sort_acc_vector_degreeMatrix(int* arr, int size);
bool is_second_list_bigger(int* arr, int size, int row1, int row2);

float distance(matrix* g1, matrix* g2) {
	int sum_acc_value = 0;
	int m_unique_diff_value;
	int g1_n = graph_get_n(g1);
	int g2_n = graph_get_n(g2);
	int g1_m_unique = graph_get_m_unique(g1);
	int g2_m_unique = graph_get_m_unique(g2);
	
	//printf("\ng1_n: %d, g2_n: %d, g1_m_unique: %d, g2_m_unique: %d\n", g1_n, g2_n, g1_m_unique, g2_m_unique);
	
	if (g1_n > g2_n) {
	
		sum_acc_value = multigraph_position_sum(g2_n, g1_n);
		m_unique_diff_value = g1_m_unique - g2_m_unique;
	}
	else if (g1_n == g2_n) 
	{
		m_unique_diff_value = abs(g1_m_unique - g2_m_unique);
	}
	else 
	{
		sum_acc_value = multigraph_position_sum(g1_n, g2_n);
		m_unique_diff_value = g2_m_unique - g1_m_unique;
	}
	//printf("sum_acc_value: %d, m_unique_diff_value: %d, abs(g1_n - g2_n): %d\n", sum_acc_value, m_unique_diff_value, abs(g1_n - g2_n));
	return  sum_acc_value + m_unique_diff_value + abs(g1_n - g2_n) + multigraph_similarity(g1, g2, g1_n, g2_n);
}

int graph_get_n(matrix* g)
{
	int size = 0;
	for (int i = 0; i < g->size; i++)
		if (g->mat[i * g->size + i] == 0)
			size++;
	return size;
}

int graph_get_m_unique(matrix* g)
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

int multigraph_position_sum(int g1_n, int g2_n)
{
	int result = 0;
	for (int i = g1_n; i < g2_n; i++)
		result += (i * (i - 1));
	return result;
}


float multigraph_similarity(matrix* g1, matrix* g2, int  g1_n, int g2_n){

	float bounds[SCALE_TAB] = {0.5, 0.625, 0.75, 0.875, 1.0}; 

	float err_outgoing_edges_acc_diff;
	float err_outgoing_edges_diff;
	float err_neighbour_outgoing_edges_acc_diff;
	int* m1_self, *m2_self, *m1_neighbour, *m2_neighbour;

	if(g1_n < g2_n)
	{
		//printf("Condition1 error: %f\n", (g2_n - g1_n) / (float)g2_n);
		return scale(bounds[3], bounds[4], (g2_n- g1_n) / (float)g2_n);
	}
	else if (g1_n > g2_n) {
		//printf("Condition1 error: %f\n", (g1_n - g2_n) / (float)g1_n);
		return scale(bounds[3], bounds[4], (g1_n - g2_n) / (float)g1_n);
	}

	m1_self = build_degree_self_matrix(g1, g1_n);
	m2_self = build_degree_self_matrix(g2, g2_n);
	
	/*array2d_print(m1_self, g1_n, g1_n, "m1_self:");
	array2d_print(m2_self, g2_n, g2_n, "m2_self:");*/

	m1_neighbour = build_degree_neighbour_matrix(g1, m1_self, g1_n);
	m2_neighbour = build_degree_neighbour_matrix(g2, m2_self, g2_n);

	sort_degreeMatrix(m1_self, g1_n);
	sort_degreeMatrix(m2_self, g2_n);

	/*array2d_print(m1_self, g1_n, g1_n, "sorted m1_self:");
	array2d_print(m2_self, g2_n ,g2_n ,"sorted m2_self:");*/

	err_outgoing_edges_acc_diff = count_err_acc_diff(m1_self, m2_self, g1_n);
	//printf("Condition2 error: %f\n", err_outgoing_edges_acc_diff);

	if (err_outgoing_edges_acc_diff != 0)
	{
		free(m1_self);
		free(m2_self);
		return scale(bounds[2], bounds[3], err_outgoing_edges_acc_diff);
	}

	err_outgoing_edges_diff = count_err_diff(m1_self, m2_self, g1_n);
	//printf("Condition3 error:  %f\n", err_outgoing_edges_diff);

	free(m1_self);
	free(m2_self);

	if (err_outgoing_edges_diff != 0)
	{
		return scale(bounds[1], bounds[2], err_outgoing_edges_diff);
	}

	/*array2d_print(m1_neighbour, g1_n, g1_n, "m1_neighbour:");
	array2d_print(m2_neighbour, g2_n, g2_n, "m2_neighbour:");*/

	sort_degreeMatrix(m1_neighbour, g1_n);
	sort_degreeMatrix(m2_neighbour, g2_n);

	/*array2d_print(m1_neighbour, g1_n, g1_n, "m1_neighbour sorted:");
	array2d_print(m2_neighbour, g2_n, g2_n, "m2_neighbour sorted:");*/

	err_neighbour_outgoing_edges_acc_diff = count_err_diff(m1_neighbour, m2_neighbour, g1_n);
	//printf("Condition4 error: %f\n", err_neighbour_outgoing_edges_acc_diff);

	free(m1_neighbour);
	free(m2_neighbour);

	if (err_neighbour_outgoing_edges_acc_diff != 0)
	{
		return scale(bounds[0], bounds[1], err_neighbour_outgoing_edges_acc_diff);
	}

	return 0;
}

float scale(float lower, float upper, float value)
{
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
	//printf("count_err_acc_diff:\n difference: %f, max_sum: %f\n", diff, max_sum);
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
	//printf("count_err_diff:\n difference: %f, max_sum: %f\n", diff, max_sum);
	return diff / max_sum;
}


void sort_degreeMatrix(int* arr, int size) {
	sort_each_vector_in_degreeMatrix(arr, size);
	sort_acc_vector_degreeMatrix(arr, size);
}

void sort_each_vector_in_degreeMatrix(int* arr, int size) {
	for (int k = 0; k < size; k++) {
		int i, j;
		int swapped;
		for (i = 0; i < size - 2; i++) {
			swapped = 0;
			for (j = 0; j < size - i - 2; j++) {
				if (arr[k * size + j] == 0) continue;
				if (arr[k * size + j] < arr[k * size + j + 1]) {
					int tmp = arr[k * size + j];
					arr[k * size + j] = arr[k * size + j + 1];
					arr[k * size + j + 1] = tmp;
					swapped = 1;
				}
			}
			if (swapped == 0)
				break;
		}
	}
}
void sort_acc_vector_degreeMatrix(int* arr, int size) {
	int i, j;
	int swapped;
	for (i = 0; i < size - 1; i++) {
		swapped = 0;
		for (j = 0; j < size - i - 1; j++) {
			if (arr[size * j + (size - 1)] < arr[size * (j + 1) + (size - 1)] ||
				(arr[size * j + (size - 1)] == arr[size * (j + 1) + (size - 1)] && is_second_list_bigger(arr, size, j, j + 1))) {
				for (int k = 0; k < size; ++k) {
					int tmp = arr[size * j + k];
					arr[size * j + k] = arr[size * (j + 1) + k];
					arr[size * (j + 1) + k] = tmp;
					swapped = 1;
				}
			}
		}
		if (swapped == 0)
			break;
	}
}

bool is_second_list_bigger(int* arr, int size, int row1, int row2)
{
	for (int i = 0; i < size - 1; i++) {
		if (arr[row1 * size + i] < arr[row2 * size + i])
			return true;
		else if (arr[row1 * size + i] > arr[row2 * size + i])
			return false;
	}
	return false;
}