#include "type.h"

void initArray(Array *a, size_t initialSize){
  a->array = (struct node *)malloc(initialSize * sizeof(struct node));
  a->used = 0;
  a->size = initialSize;
}

int insertArray(Array *a, int id, int E){
  if(search(a,id,E) == -1){
   //printf("returning -1. found dup\n");
    return -1;
  }
  if(a->used == a->size){
    a->size *=2;
    a->array = (struct node *)realloc(a->array, a->size * sizeof(struct node));
    //printf("Resizing array\n");
  }
  struct node *newNode = (struct node*)malloc(sizeof(struct node));
  newNode->id = id;
  newNode->next = E;
  a->array[a->used++] = *newNode;
  return 1;
}

void freeArray(Array *a){
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

int search(Array *a, int V, int E){
  int i;
  for(i = 0; i < a->used; i++){
    //printf("V: %d id: %d\n",V, a->array[i].id);
    if( (a->array[i].id == V) && (a->array[i].next == E)){
      //printf("duplicate\n");
      return -1;
    }
  }
  return 1;
}

//prototypes
int readFile(char *fileName);

int V,E;
Array a;


int main(int argc, char*argv[]){
//first read the file
initArray(&a,5);
readFile("graph1.txt");
}


int readFile(char *fileName){
  FILE *fp = fopen(fileName, "r+");
  if(!fp){
    printf("could not open file\n");
    exit(1);
  }
  int i = 0;
  while(fscanf(fp, "%d %d", &V,&E)==2){
   // printf("V: %d E:%d\n",V,E);
    insertArray(&a, V,E);
   // printf("%d -> %d\n",a.array[i].id,a.array[i].next);
    i++;
  }
    printf("used: %d\n", a.used);
    freeArray(&a);
}

