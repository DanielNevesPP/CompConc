#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

int main(int argc, char* argv[]){

    int dim, nThreads;

    if(argc  < 3){
      printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
      return 1;
    }

    dim = atoi(argv[1]);
    nThreads = atoi(argv[2]);

}
