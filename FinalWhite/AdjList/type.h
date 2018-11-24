#include <stdio.h>
#include <stdlib.h>

//graph type
typedef struct graph *Graph;
//create graph of initial size n
Graph crete_graph(int n);
//destroy the graph
void decimate_graph(Graph);
//create an edge
void add_edge(Graph g, int u, int v);
//get graph counts
int vertex_count(Graph);
int edge_count(Graph);
//get out-degree of a node  1 == yes 0 == no
int out_deg(Graph, struct node source);
//check if an edge exist
int has_edge(Graph, int source, int sink);
//invoke a function on the graph, we will be doing this!
void graph_invoke(Graph g, int source, void (*f)(Graph g, int source, int sink, void *data),void *data);
