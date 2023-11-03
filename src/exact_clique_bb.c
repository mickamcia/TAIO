#include "exact_clique_bb.h"
#include "graph.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define DEBUG 0


int graph_weight_selected_vertices(const matrix* g, const int* vertices){
    int total = 0;
    for(int i = 0; i < g->size; i++){
        if(vertices[i] == 0) continue;
        for(int j = 0; j < g->size; j++){
            if(vertices[j] == 0) continue;
            total += g->mat[i * g->size + j];
        }
    }
    return total;
}

int is_empty(const int* set, const int size){
    for(int i = 0; i < size; i++){
        if(set[i] != 0) return 0;
    }
    return 1;
}

int is_addable_to_clique(const matrix* g, const int* clique, const int vertex){
    if(clique[vertex] == 1) return 0;

    for(int i = 0; i < g->size; i++){
        if(clique[i] == 0) continue;
        for(int j = 0; j < g->size; j++){
            if(j == vertex) continue;
            if(g->mat[i * g->size + vertex] == 0) return 0;
        }
    }
    return 1;
}

void calculate_candidates(const matrix* g, const int* clique, int* candidates){
    for(int i = 0; i < g->size; i++){
        if(is_addable_to_clique(g, clique, i)){
            candidates[i] = 1;
        }
        else{
            candidates[i] = 0;
        }
    }
}

void calculate_expandable(int* expandable, const int* nbors, const int* candidates, const int* permutation, const int vertex, const int size){
    for(int j = 0; j < size; j++){
        expandable[j] = nbors[j] * candidates[j];
    }
    for(int j = 0; j <= vertex; j++){
        if(permutation[j] != -1){
            expandable[permutation[j]] = 0;
        }
    }
}

void calculate_nbors(const matrix* g, int* nbors, const int vertex){
    for(int i = 0; i < g->size; i++){
        if(i != vertex && g->mat[i * g->size + vertex] != 0){
            nbors[i] = 1;
        }
        else{
            nbors[i] = 0;
        }
    }
}

int calculate_sigma_weight(const matrix* g, const int* clique, const int vertex){
    int total = 0;
    for(int i = 0; i < g->size; i++){
        if(clique[i] == 1){
            total += g->mat[i * g->size + vertex];
        }
    }
    return total;
}

void calculate_permutation_and_upper_bound(const matrix* g, const int* curr_clique, const int* candidates, int* permutation, int* upper_bound){
    int* nbors = (int*)malloc(sizeof(int) * g->size);
    int* sigma_scores = (int*)malloc(sizeof(int) * g->size);
    int* candidates_prim = (int*)malloc(sizeof(int) * g->size);
    int* vertex_coloring = (int*)malloc(sizeof(int) * g->size);
    int* indep_set_cands = (int*)malloc(sizeof(int) * g->size);

    int fill_index = g->size - 1;

    for(int i = 0; i < g->size; i++){
        vertex_coloring[i] = -1;
        upper_bound[i] = -1;
        permutation[i] = -1;
        sigma_scores[i] = calculate_sigma_weight(g, curr_clique, i);
        candidates_prim[i] = candidates[i];
    }
    int k = 0;

    while(!is_empty(candidates_prim, g->size)){
        if(DEBUG) printf("\nK: %d", k);
        k++;
        for(int i = 0; i < g->size; i++){
            indep_set_cands[i] = candidates_prim[i];
        }
        while(!is_empty(indep_set_cands, g->size)){
            // find min_vertex
            int min_vertex = -1;
            for(int i = 0; i < g->size; i++){
                if(indep_set_cands[i] == 0) continue;
                if(min_vertex == -1 || sigma_scores[min_vertex] > sigma_scores[i]){
                    min_vertex = i;
                }
            }
            if(DEBUG) printf("\nMIN_VERTEX: %d", min_vertex);
            // calculate upper_bound for min_vertex
            upper_bound[min_vertex] = sigma_scores[min_vertex]; // + max
            for(int color = 0; color < k; color++){
                int max_score = 0;
                for(int i = 0; i < g->size; i++){
                    if(vertex_coloring[i] == color){
                        if(max_score < sigma_scores[i]){
                            max_score = sigma_scores[i];
                        }
                    }
                }
                upper_bound[min_vertex] += max_score;
            }

            // update coloring with min_vertex
            vertex_coloring[min_vertex] = k;

            // append min_vetrex to permutation
            permutation[fill_index--] = min_vertex;

            // remove nbors of min_vertex from indep_set_cands
            calculate_nbors(g, nbors, min_vertex);

            if(DEBUG){                
                printf("\nindep_set_cands:  ");
                for(int i = 0; i < g->size; i++){
                    printf("%d ", indep_set_cands[i]);
                }
                printf("\nnbors:            ");
                for(int i = 0; i < g->size; i++){
                    printf("%d ", nbors[i]);
                }
                printf("\ncandidates_prim:  ");
                for(int i = 0; i < g->size; i++){
                    printf("%d ", candidates_prim[i]);
                }
                printf("\n");
            }
            for(int i = 0; i < g->size; i++){
                if(nbors[i] == 1){
                    indep_set_cands[i] = 0;
                }
            }
            // not mentioned in the paper
            indep_set_cands[min_vertex] = 0;

            // remove min_vertex from candidates_prim
            candidates_prim[min_vertex] = 0;
        }

        for(int i = 0; i < g->size; i++){
            // update sigma_scores for each vertex in candidates_prim
            if(candidates_prim[i] == 0) continue;
            calculate_nbors(g, nbors, i);
            for(int j = 0; j < g->size; j++){
                if(vertex_coloring[j] != k){
                    nbors[j] = 0;
                }
            }
            int max_score = 0;
            for(int j = 0; j < g->size; j++){
                if(nbors[j] == 1){
                    if(max_score < g->mat[i * g->size + j]){
                        max_score = g->mat[i * g->size + j];
                    }
                }
            }
            sigma_scores[i] = sigma_scores[i] + max_score;
        }
    }

    free(nbors);
    free(sigma_scores);
    free(candidates_prim);
    free(vertex_coloring);
    free(indep_set_cands);
}

void expand(const matrix* g, int* best_clique, int* curr_clique, int* candidates){

    if(DEBUG){  
        printf("\nENTER");
        printf("\nbest_clique: ");
        for(int i = 0; i < g->size; i++){
            printf("%d ", best_clique[i]);
        }
        printf("\ncurr_clique: ");
        for(int i = 0; i < g->size; i++){
            printf("%d ", curr_clique[i]);
        }
        printf("\ncandidates:  ");
        for(int i = 0; i < g->size; i++){
            printf("%d ", candidates[i]);
        }
        printf("\n");
    }

    const int curr_clique_score = graph_weight_selected_vertices(g, curr_clique);
    const int best_clique_score = graph_weight_selected_vertices(g, best_clique);
    if(DEBUG) printf("\nSCORES: %d %d", best_clique_score, curr_clique_score);
    if(is_empty(candidates, g->size)){
        if(curr_clique_score >= best_clique_score){
            memcpy(best_clique, curr_clique, sizeof(int) * g->size);
        }
        if(DEBUG) printf("\nTERMINATE: %d %d", best_clique_score, curr_clique_score);
        return;
    }

    int* expandable = (int*)malloc(sizeof(int) * g->size);
    int* nbors = (int*)malloc(sizeof(int) * g->size);
    int* permutation = (int*)malloc(sizeof(int) * g->size);
    int* upper_bound = (int*)malloc(sizeof(int) * g->size);

    //calculate_candidates(g, curr_clique, candidates);
    calculate_permutation_and_upper_bound(g, curr_clique, candidates, permutation, upper_bound);
    if(DEBUG){  
        printf("\npermutation: ");
        for(int i = 0; i < g->size; i++){
            printf("%d ", permutation[i]);
        }
        printf("\nupper_bound: ");
        for(int i = 0; i < g->size; i++){
            printf("%d ", upper_bound[i]);
        }
        printf("\n");
    }
    for(int i = 0; i < g->size; i++){
        const int idx = permutation[i];
        if(idx < 0) continue;
        if(candidates[idx] == 1 && curr_clique_score + upper_bound[idx] > best_clique_score){
            curr_clique[idx] = 1;
            if(DEBUG) printf("\nPERM UPPER: %d %d", permutation[i], upper_bound[idx]);
            calculate_nbors(g, nbors, idx);
            calculate_expandable(expandable, nbors, candidates, permutation, i, g->size);
            if(DEBUG){
                printf("\nnbors:       ");
                for(int j = 0; j < g->size; j++){
                    printf("%d ", nbors[j]);
                }
                printf("\ncandidates:  ");
                for(int j = 0; j < g->size; j++){
                    printf("%d ", candidates[j]);
                }
                printf("\nexpandable:  ");
                for(int j = 0; j < g->size; j++){
                    printf("%d ", expandable[j]);
                }
            }
            expand(g, best_clique, curr_clique, expandable);
            curr_clique[idx] = 0;
        }
    }

    free(expandable);
    free(nbors);
    free(permutation);
    free(upper_bound);
}

// driver function
void exact_clique_bb_run(matrix *g){

    graph_simplify_multidigraph_to_multigraph(g);
    graph_print(g);
    int* curr_clique = (int*)malloc(sizeof(int) * g->size);
    int* best_clique = (int*)malloc(sizeof(int) * g->size);
    int* candidates  = (int*)malloc(sizeof(int) * g->size);
    for(int i = 0; i < g->size; i++){
        curr_clique[i] = 0;
        best_clique[i] = 0;
        candidates[i] = 1;
    }
    
    expand(g, best_clique, curr_clique, candidates);

   

    for(int i = 0; i < g->size; i++){
        for(int j = 0; j < g->size; j++){
            if(best_clique[i] == 0 || best_clique[j] == 0){
                g->mat[i * g->size + j] = -1;
            }
        }
    }

    free(curr_clique);
    free(best_clique);
    free(candidates);
}