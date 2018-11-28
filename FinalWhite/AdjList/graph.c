#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>
int numNodes;
int top5[5];
int visits = 0;
int wentLocal = 0;
int wentRandom = 0;
struct node{
  int visited;
  int vertex;
  struct node *next;
};

struct node* createNode(int);

struct Graph{
  int numV;
  struct node **adjLists;
  int totalVisits;
};

struct Graph *createGraph(int vertices);
void addEdge(struct Graph* graph, int src, int dest);
void printGraph(struct Graph* graph);
int readFile(struct Graph *g, char *fileName);
int outDeg(struct node *Node);

struct node *createNode(int v){
  struct node *newNode = malloc(sizeof(struct node));
  newNode->vertex = v;
  newNode->next = NULL;
  newNode->visited = 0;
  return newNode;
}

struct Graph *createGraph(int vertices){
  struct Graph *graph = malloc(sizeof(struct Graph));
  graph->numV = vertices;
  graph->adjLists = malloc(vertices * sizeof(struct node));
  graph->totalVisits = 0;
  int i;

#pragma omp parallel for private(i)
  for(i = 0; i < vertices; i++){
    graph->adjLists[i] = NULL;
    struct node *newNode = createNode(i);
    newNode->next = graph->adjLists[i];
    graph->adjLists[i] = newNode;
  }
  return graph;

}

void addEdge(struct Graph *graph,int src, int dest){

  struct node *newNode = createNode(dest); 
  if(graph->adjLists[src] == NULL){
    newNode->next = NULL;
    graph->adjLists[src] = newNode;
  }
  struct node *cur = graph->adjLists[src];
  while(cur->next != NULL){
    cur = cur->next;
  }
  cur->next = newNode;

  //printf("visited:%d\n",newNode->visited);
  //newNode->next = graph->adjLists[src];
  //graph->adjLists[src] = newNode;

  //newNode = createNode(src);
  //newNode->next = graph->adjLists[dest];
  //graph->adjLists[dest] = newNode;
}
void print5(struct Graph *g){
  int i;
  printf("Total Visits: %d\tlocal:%d\trandom:%d\n",visits,wentLocal,wentRandom);
  printf("Top 5 nodes\n");
  printf("Node\tvisited % \n");
  for(i=0; i<5; i++){
    struct node *temp = g->adjLists[top5[i]];
    printf("%d\t%f\n",temp->vertex,((double)temp->visited /(double) visits));
  }


}
void printGraph(struct Graph *graph){
  int v,i;
  for(v=0; v < graph->numV; v++){
    struct node *temp = graph->adjLists[v];
    printf("List of V: %d ",temp->vertex);
    //printf("outdeg is: %d\n",outDeg(temp));
    i = 0;
    printf("%d visited %d times\n", temp->vertex, temp->visited);
    while(temp){
      // printf("i:%d\t",i);
      printf("%d -> ",temp->vertex);
      temp = temp->next;
      i++;
    }
    putchar(10);
  }
}
//function to check the top 5 values in the graph
int checkTop(struct Graph *g){
  int v;
  for(v = 0;v<g->numV; v++){ 
    if(g->adjLists[v]->visited > g->adjLists[top5[0]]->visited){
      //printf("%d > %d\n",g->adjLists[v]->visited,g->adjLists[top5[j]]->visited);
      //trickleDown(j,v);
      top5[4] = top5[3];
      top5[3] = top5[2];
      top5[2] = top5[1];
      top5[1] = top5[0];
      top5[0] = v;

    }else if(g->adjLists[v]->visited > g->adjLists[top5[1]]->visited){

      top5[4] = top5[3];
      top5[3] = top5[2];
      top5[2] = top5[1];
      top5[1] = v;
    }else if(g->adjLists[v]->visited > g->adjLists[top5[2]]->visited){
      top5[4] = top5[3];
      top5[3] = top5[2];
      top5[2] = v;
    }else if(g->adjLists[v]->visited > g->adjLists[top5[3]]->visited){

      top5[4] = top5[3];
      top5[3] = v;

    }else if(g->adjLists[v]->visited > g->adjLists[top5[4]]->visited){
      top5[4] = v;
    }
  }
}

int readFile(struct Graph *g, char *fileName){
  FILE *fp = fopen(fileName, "r+");
  if(!fp){
    printf("could not open file\n");
    exit(1);
  }
  int i, V,E;

  while(fscanf(fp, "%d %d", &V,&E)==2){
    // printf("V: %d E:%d\n",V,E);
    addEdge(g, V,E);
    i++;
  }
}

int randomWalk(struct Graph *g,int j, int k, double d){
  int i,seed,rNum,out;
  struct node *temp = g->adjLists[j];
 // temp->visited++;
  //int visits = g->totalVisits++;
  // #pragma omp parallel for private(i,seed,rNum,out)
  //#pragma omp parallel
  {
    //#pragma omp set_num_threads(6) 
    //srand(time(NULL));
#pragma omp parallel for private(i,out,rNum,seed) reduction(+:visits)
    for(i = 0; i<k; i++){
      struct node *temp2 = g->adjLists[j];
      //printf("%d\n",i);
      // printf("CURRENT NODE: %d\n",temp->vertex);
      // printf("%d has been visited %d times\n",temp->vertex, temp->visited);
      out = outDeg(temp2);
      // printf("out is:%d\n",out);
      //create random node
      seed = time(NULL);
      //seed =  omp_get_thread_num() + 1;
      seed *= i;
      rNum = rand_r(&seed)% 100 + 1;
      if(out == 0){
        rNum = d;
      }
      if(rNum<=(d*100)){
        #pragma omp atomic
        wentRandom++;
        //  printf("its heads! pick a random node.\n");
        rNum = (rand_r(&seed)%(numNodes-1)+1);//rand_r(&seed)% numNodes;
        temp2 = g->adjLists[rNum];
        //printf("random rNum is %d\n",rNum);
       // printf("temp is: %d\n", temp2->vertex);
        visits++;
        //#pragma omp atomic
        temp2->visited++;
       // printf("new visited:%d\n",temp2->visited);
        // printf("new temp is: %d\n",temp->vertex);
      }else{
        #pragma omp atimoic
        wentLocal++;
        // printf("its tails! Choose from local.\n");
        //printf("temp->vertex: %d\n", temp2->vertex);
        rNum = rand_r(&seed) % (out) +1; // rand_r(&seed)% (out) +1;
       // printf("local rnum is: %d\n",rNum);
        int q = 0;
        while(q < rNum && temp2){
          temp2 = temp2->next;
          q++;
        }
         //printf("temp is: %d\n", temp2->vertex);
        //#pragma omp atomic
        visits++;
        //#pragma omp atomic
        temp2->visited++;
       // printf("new visited:%d\n",temp2->visited);
      }
      // printf("new rand is ajd[%d] -> num %d\n",temp->vertex, rNum);
    }
  }
  // printf("go to node %d !\n",rNum);
  //  g->adjLists[rNum]->visited++;
}





/***********************************************************************
  void swap(struct Graph *g, int a, int b){
// printf("SWAPPING %d and %d\n",a,b);
struct node *temp = g->adjLists[a];
g->adjLists[a] = g->adjLists[b];
g->adjLists[b] = temp;
}
int partition(struct Graph *g, int low, int high){
int piv = g->adjLists[high]->visited;
// int piv = (g->adjLists[low]->visited + g->adjLists[numNodes/2]->visited + g->adjLists[high]->visited)/3;

// printf("PIV:%d\n",piv);
int i = (low - 1);
int j;
//#pragma omp parallel for private(j)
for(j = low; j<=high -1; j++){
// printf("list j = %d, visited=%d\n",g->adjLists[j]->vertex,g->adjLists[j]->visited);
if(g->adjLists[j]->visited<=piv){
i++;
swap(g,i,j);}
}
swap(g,i+1,high);
return i+1;
}
int quicksort(struct Graph *g, int low, int high){
//quicksort, graph, starting and ending index
if(low < high){
int pi = partition(g,low, high);
#pragma omp task
quicksort(g, low, pi-1);
#pragma omp task
quicksort(g, pi+1, high);
}
}
 ***************************************************************************/

int outDeg(struct node *Node){
  int i= 0;
  struct node *temp = Node;
  while(temp){
    i++;
    temp = temp->next;
  }
  return i-1;
}

int main(int argc, char *argv[]){
  char *fileName, preProc[128];
  FILE *fp;
  char *temp;
  int num,k;
  double damp;
  if(argc < 5){
    printf("Usage: numNodes walkLength dampRatio fileName\n");
    exit(1);
  }
  memset(top5,0,5);
  numNodes = atoi(argv[1]);
  fileName = argv[4];
  k = atoi(argv[2]);
  damp = atof(argv[3]);
  struct Graph *graph = createGraph(numNodes);
  readFile(graph, fileName);
  // printGraph(graph);
  int i;
  double start = omp_get_wtime();
  printf("starting walks\n");
  //#pragma omp parallel for private(i)
  for(i = 0; i < numNodes; i++){
    //printf("%d\n",i);
    randomWalk(graph,i, k,damp);
  }
  /*
  //printf("Starting quicksort\n");
#pragma omp parallel 
{
#pragma omp single nowait
{
quicksort(graph, 0, numNodes-1);
}
}
*/
//printf("checking top5\n");
checkTop(graph);
double total = omp_get_wtime() - start;
printf("Total Time: %f\n",total);
print5(graph);
//printGraph(graph);
return 0;
}
