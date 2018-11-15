#include "fun.h"
double xc = 0.5;
double yc = 0.5;
double r = 0.5;

int inCirc(double xp, double yp){
  double xdif = xp - xc;
  double ydif = yp - yc;
  double d;
  d = ( xdif * xdif) + (ydif * ydif);
 // printf("d: %f, r^2 : %f
  if(d<(r*r))
    return 1;
  if(d>(r*r))
    return 0;

  return 0;
}

