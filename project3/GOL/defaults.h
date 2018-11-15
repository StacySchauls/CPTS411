#include <stddef.h>
#include <stbool.h>
#include <getopt.h>



const int BIG_PRIME = 93377;
//the data that willbe used for the instace of life
struct life_t {
  int rank;
  int size;
  int ncols;
  int nrows;
  int **grid;
  int **next_grid;
  int generations;

}

struct cell_state {dead = 0, alive = 1};
