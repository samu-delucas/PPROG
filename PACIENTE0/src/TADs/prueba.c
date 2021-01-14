#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int aleat_num_mod(int inf, int sup, int *num)
{
  int aleatorio, flag, i;

	/*Si no queremos que haya negativos y que la difererncia sea menor de 1*/
	do{
	flag=0;
  	if(inf<0 ||sup-inf<0) return -1;
  	aleatorio=(inf+(int)(sup-inf+1.0)*(rand()/(RAND_MAX+1.0)));
  
  	for(i=0;i<6;i++){
  		if(aleatorio==num[i]) flag=1;
  		}
	}while(flag==1);
	
  return aleatorio;
}

int main(){
	
	    int i, aux[6], n;
            
              srand(time(NULL));
	
		for(i=0;i<6;i++){
		n=aleat_num_mod(0,10-1,aux);
		printf("%d\n", n);
		fflush(stdout);
		aux[i]=n;
	}

}


