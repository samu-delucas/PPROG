#include <stdio.h>
#include <stdlib.h>
#include "types.h"

int aleat_num(int inf, int sup)
{
  int aleatorio;

  if(inf<0 ||sup-inf<0) return -1;
  aleatorio=(inf+(int)(sup-inf+1.0)*(rand()/(RAND_MAX+1.0)));
  return aleatorio;
}
