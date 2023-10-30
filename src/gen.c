#include "graph.h"
#include <time.h>
#include <stdlib.h>

int main(){
    srand(time(NULL));

    matrix* g1 = matrix_init(8);
    graph_generate(g1, 5, 2, 0.5);
    
    matrix* g2 = matrix_clone(g1);
    graph_add_noise(g2, 0.5, 1, 0.0);

    matrix* g3 = matrix_clone(g2);
    graph_permute(g3);
    
    graph_print(g1);
    graph_print(g2);
    graph_print(g3);
    
    graph_save_to_file(g1, "res/G1.txt");
    graph_save_to_file(g2, "res/G2.txt");
    graph_save_to_file(g3, "res/G3.txt");
    
    matrix_destroy(g1);
    matrix_destroy(g2);
    matrix_destroy(g3);
}
