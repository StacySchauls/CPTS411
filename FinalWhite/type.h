#include <stdio.h>
#include <stdlib.h>


struct node{
  int id;
  int next;
  int visted;
};

typedef struct{
 struct node *array;
  size_t used;
  size_t size;
} Array;
int search(Array *a,int V, int E);
void initArray(Array *a, size_t initialSize);
void freeArray(Array *a);
int readFile(char *fileName);
int insertArray(Array *a, int id, int E);
