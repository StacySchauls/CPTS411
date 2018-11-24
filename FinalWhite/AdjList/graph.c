#include "type.h"
int V,E;
struct node{
  int id;
  int next;
  int visted;
};

struct graph{
  int n; //number of nodes
  int m;  //number of edges
  struct successors{
    int d; //number of successors
    int len;//len of array
    char is_sorted;//true if list is sorted
    struct node list[1]; //the l ist of successors
  }*alist[1];
};

Graph creat_graph(int n){
Graph g;
  int i;
  g = malloc(sizeof(struct graph) + sizeof(struct successors *) *( n-1));
  g->n = n;
  g->m = 0;
  
  for(i = 0; i < n; i++){
    g->alist[i] = malloc(sizeof(struct successors));
    g->alist[i]->d = 0;
    g->alist[i]->len = 1;
    g->alist[i]->is_sorted = 1;
  }
  return g;
}

add_edge(Graph g, int u, int v){

    while(g->alist[u]->d >= g->alist[u]->len){
      g->alist[u]->len *=2;
      g->alist[u] = realloc(g->alist[u], sizeof(struct successors) + sizeof(struct node) * (g->alist[u]->len-1));
    }
    g->alist[u]->list[g->alist[u]->d] = v;
    g->alist[u]->is_sorted = 0;

    g->m++;

}
 int readFile(char *fileName){
    FILE *fp = fopen(fileName, "r+");
     if(!fp){
      printf("could not open file\n");
      exit(1);
    }
    int i = 0;
    Graph g = create_graph(5);
    while(fscanf(fp, "%d %d", &V,&E)==2){
     // printf("V: %d E:%d\n",V,E);
      insertArray(g, V,E);
     // printf("%d -> %d\n",a.array[i].id,a.array[i].next);
      i++;
    }
     //printf("used: %d\n", a.used);
     
}

