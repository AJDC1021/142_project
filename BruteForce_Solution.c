#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


typedef struct Node {
    int neighbor_id;
    struct Node *next_node;
} GraphNode;

GraphNode **build_adjacency_list(int *vertex_count, int *edge_count);
bool check_if_adjacent(GraphNode **adj_list, int vertex_u, int vertex_v);
void brute_force_clique_search(GraphNode **adj_list, int total_vertices, int *current_assignments, int vertex_index, int cliques_used, int *min_cliques_found);
bool is_valid_partition(GraphNode **adj_list, int total_vertices, int *assignments, int cliques_used);

int main() {
    GraphNode **adjacency_list;
    int total_vertices, total_edges;
    int min_cliques_found;
    
    adjacency_list = build_adjacency_list(&total_vertices, &total_edges);
    if (adjacency_list == NULL) return 1;

    min_cliques_found = total_vertices; 
    int *clique_assignments = (int *)malloc(sizeof(int) * total_vertices);
    
    clock_t start_time = clock(); // Start the clock


    printf("Search started...\n");

    // Execute brute force search
    brute_force_clique_search(adjacency_list, total_vertices, clique_assignments, 0, 0, &min_cliques_found);

    // 3. Stop the clock
    clock_t end_time = clock();

    // 4. Calculate duration in seconds
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("\nMinimum number of cliques: %d\n", min_cliques_found);
    // Showing 15 decimal places for extreme precision
    printf("Execution time: %.10f seconds\n", time_taken);

    free(clique_assignments);
    return 0;
}

bool check_if_adjacent(GraphNode **adj_list, int vertex_u, int vertex_v) {
    GraphNode *current = adj_list[vertex_u];
    while (current != NULL) { // Check if adjacent
        if (current->neighbor_id == vertex_v) 
            return true;
        current = current->next_node;
    }
    return false;
}

bool is_valid_partition(GraphNode **adj_list, int total_vertices, int *assignments, int cliques_used) {
    for (int v1 = 0; v1 < total_vertices; v1++) {
        for (int v2 = v1 + 1; v2 < total_vertices; v2++) {
            // If two vertices are assigned to the same clique ID
            if (assignments[v1] == assignments[v2]) {
                // They MUST be adjacent in the original graph
                if (!check_if_adjacent(adj_list, v1, v2)) {
                    return false; 
                }
            }
        }
    }
    return true;
}


void brute_force_clique_search(GraphNode **adj_list, int total_vertices, int *current_assignments, 
    int vertex_index, int cliques_used, int *min_cliques_found) {
    
    // Base Case: A complete set partition has been generated.
    if (vertex_index == total_vertices) {
        
        // After generating the partition, check if every group in it forms a clique
        if (is_valid_partition(adj_list, total_vertices, current_assignments, cliques_used)) {
            
            // If the partition is valid, check if it uses fewer cliques
            if (cliques_used < *min_cliques_found) {
                *min_cliques_found = cliques_used;
            }
        } 
        return;
    }

    // Recursive Step: Try placing the current vertex into every existing clique.
    // This branching factor is what causes the exponential time growth seen in the data table.
    for (int clique_id = 0; clique_id < cliques_used; clique_id++) {
        current_assignments[vertex_index] = clique_id;
        brute_force_clique_search(adj_list, total_vertices, current_assignments, vertex_index + 1, 
            cliques_used, min_cliques_found);
    }

    // Branching Step: Also try placing the current vertex into a brand-new clique.
    current_assignments[vertex_index] = cliques_used;
    brute_force_clique_search(adj_list, total_vertices, current_assignments, vertex_index + 1, 
        cliques_used + 1, min_cliques_found);
}

GraphNode **build_adjacency_list(int *vertex_count, int *edge_count) {
    // Initialization
    int i, u, w; 
    GraphNode **adj_list;
    if (scanf("%d %d", vertex_count, edge_count) != 2) return NULL;
    adj_list = (GraphNode **)malloc(sizeof(GraphNode *) * (*vertex_count));
    for (i = 0; i < *vertex_count; i++) adj_list[i] = NULL;
    
    for (i = 0; i < *edge_count; i++) {
        if (scanf("%d %d", &u, &w) != 2) break;
        
        GraphNode *new_node_u = (GraphNode *)malloc(sizeof(GraphNode));
        new_node_u->neighbor_id = w;
        new_node_u->next_node = adj_list[u];
        adj_list[u] = new_node_u;

        GraphNode *new_node_w = (GraphNode *)malloc(sizeof(GraphNode));
        new_node_w->neighbor_id = u;
        new_node_w->next_node = adj_list[w];
        adj_list[w] = new_node_w;
    }
    return adj_list;
}