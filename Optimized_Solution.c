#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct Node {
    int neighbor_id;
    struct Node *next_node;
} GraphNode;

GraphNode **build_adjacency_list(int *vertex_count, int *edge_count);
GraphNode **create_complement_graph(GraphNode **original_list, int vertex_count);
int *greedy_graph_coloring(GraphNode **adj_list, int vertex_count);
int find_available_color(GraphNode **adj_list, int *color_assignments, int vertex_count, int current_vertex);
void display_results(int *color_assignments, int vertex_count, double time_spent);
void free_graph(GraphNode **adj_list, int vertex_count);

int main() {

    // Initialization
    GraphNode **original_graph, **complement_graph;
    int total_vertices, total_edges;
    int *clique_partition;
    original_graph = build_adjacency_list(&total_vertices, &total_edges);
    if (original_graph == NULL) return 1;

    printf("Search started...\n");

    clock_t start_clock = clock(); // Start Clock

    // Create complement, then start greedy algorithm
    complement_graph = create_complement_graph(original_graph, total_vertices);
    clique_partition = greedy_graph_coloring(complement_graph, total_vertices);

    clock_t end_clock = clock(); // End Clock
    double time_spent = (double)(end_clock - start_clock) / CLOCKS_PER_SEC;

    display_results(clique_partition, total_vertices, time_spent);

    free_graph(original_graph, total_vertices);
    free_graph(complement_graph, total_vertices);
    free(clique_partition);

    return 0;
}


int *greedy_graph_coloring(GraphNode **adj_list, int vertex_count) {

    // Initialize Colors
    int *colors = (int *)malloc(sizeof(int) * vertex_count);
    for (int i = 0; i < vertex_count; i++) 
        colors[i] = -1;

    colors[0] = 0; // Mark Starting Color
    for (int i = 1; i < vertex_count; i++) {
        colors[i] = find_available_color(adj_list, colors, vertex_count, i); // Find next color, 
        // then assign it 
    }
    return colors; // Return Colors Array
}

int find_available_color(GraphNode **adj_list, int *color_assignments, int vertex_count, int current_vertex) {
    
    // initialize unavailability array
    bool *is_unavailable = (bool *)calloc(vertex_count, sizeof(bool));

    // Pointer to current vertex
    GraphNode *p = adj_list[current_vertex];

    while (p != NULL) { 
        // Retrieve the color already assigned to the neighbor
        int neighbor_color = color_assignments[p->neighbor_id];
        
        // If the neighbor has been processed (color != -1), skip
        if (neighbor_color != -1) {
            is_unavailable[neighbor_color] = true;
        }
        p = p->next_node;
    }

    // Search for the smallest non-negative integer color that hasn't been blocked
    for (int i = 0; i < vertex_count; i++) {
        if (!is_unavailable[i]) {
            free(is_unavailable); 
            return i;
        }
    }

    free(is_unavailable);
    return 0;
}


void display_results(int *color_assignments, int vertex_count, double time_spent) { // Display Results
    int max_color = 0;
    for (int i = 0; i < vertex_count; i++) {
        if (color_assignments[i] > max_color) 
            max_color = color_assignments[i];
    }
    
    printf("\nMinimum number of cliques: %d\n", max_color + 1);
    printf("Execution time: %.10f seconds\n", time_spent);
}


GraphNode **create_complement_graph(GraphNode **original_list, int vertex_count) {

    // Complement Graph Memory Allocation
    GraphNode **complement = (GraphNode **)malloc(sizeof(GraphNode *) * vertex_count); 
    for (int i = 0; i < vertex_count; i++) 
        complement[i] = NULL;

    // A temporary boolean array to track connections for the current vertex
    bool *is_neighbor = (bool *)calloc(vertex_count, sizeof(bool));

    for (int u = 0; u < vertex_count; u++) {

        // 1. Mark existing neighbors of u from the original graph
        for (GraphNode *temp = original_list[u]; temp != NULL; temp = temp->next_node) {
            is_neighbor[temp->neighbor_id] = true;
        }

        // 2. Iterate through all vertices 'v'. If 'v' is NOT a neighbor and NOT itself,
        // it belongs in the complement graph.
        for (int v = 0; v < vertex_count; v++) {
            if (u != v && !is_neighbor[v]) {
                GraphNode *newNode = (GraphNode *)malloc(sizeof(GraphNode));
                newNode->neighbor_id = v;
                newNode->next_node = complement[u];
                complement[u] = newNode;
            }
        }

        // 3. Reset the boolean array for the next vertex iteration
        for (GraphNode *temp = original_list[u]; temp != NULL; temp = temp->next_node) {
            is_neighbor[temp->neighbor_id] = false;
        }
    }
    free(is_neighbor);
    return complement; // Return Complement Graph
}

GraphNode **build_adjacency_list(int *vertex_count, int *edge_count) {

    // Read details
    if (scanf("%d %d", vertex_count, edge_count) != 2) 
        return NULL;

    // Initialize adjacency list
    GraphNode **adj_list = (GraphNode **)malloc(sizeof(GraphNode *) * (*vertex_count)); 
    for (int i = 0; i < *vertex_count; i++) 
        adj_list[i] = NULL;


    for (int i = 0; i < *edge_count; i++) {
        int u, w;

        if (scanf("%d %d", &u, &w) != 2) 
            break;
        

        // Building Proper
        GraphNode *n1 = malloc(sizeof(GraphNode));
        n1->neighbor_id = w; 
        n1->next_node = adj_list[u]; 
        adj_list[u] = n1;

        GraphNode *n2 = malloc(sizeof(GraphNode));
        n2->neighbor_id = u; 
        n2->next_node = adj_list[w]; 
        adj_list[w] = n2;
    }
    return adj_list;
}

void free_graph(GraphNode **adj_list, int vertex_count) {
    for (int i = 0; i < vertex_count; i++) {
        GraphNode *curr = adj_list[i];
        while (curr) {
            GraphNode *temp = curr;
            curr = curr->next_node;
            free(temp);
        }
    }
    free(adj_list);
}