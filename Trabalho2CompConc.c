#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float *matriz1;
float *matriz2;
float *matrizsaida;

int main(int argc, char* argv[]){

    int dim, nthreads, i, j, k;
    double inicio, fim, delta;

    //leitura e avaliacao dos parametros de entrada
    if(argc  < 3){
      printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
      return 1;
    }
    dim = atoi(argv[1]);
    nThreads = atoi(argv[2]);
    if (nthreads > dim){
      nthreads = dim;
    }

   //alocacao de memoria para as estruturas de dados
   matriz1 = (float *) malloc(sizeof(float) * dim * dim);
   if (matriz1 == NULL) {printf("ERRO--malloc\n"); return 2;}
   matriz2 = (float *) malloc(sizeof(float) * dim * dim);
   if (matriz2 == NULL) {printf("ERRO--malloc\n"); return 2;}
   matrizsaida = (float *) malloc(sizeof(float) * dim * dim);
   if (matrizsaida == NULL) {printf("ERRO--malloc\n"); return 2;}

    //dando valores aleatorios as matrizes
    srand(time(NULL));
    for(i = 0; i < dim; i++){
      for(j = 0; j< dim; j++){
        matriz1[i * dim + j] = rand() % 10;
        matriz2[i * dim + j] = rand() % 10;
      }
    } 

    //inicializando a matriz de saida
    for(i = 0; i < dim; i++){
      for(j = 0; j < dim; j++){
        matrizsaida[i * dim + j] = 0;
      }
    }

    GET_TIME(inicio);
    //multiplicacao de matriz sequencial
    for(i = 0; i < dim; i++){
      for(j = 0; j< dim; j++){
        for(k = 0; k < dim; k++){
          matrizsaida[i * dim + j] += matriz1[i * dim + k] * matriz2[k * dim + j];
        }
      }
    }
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo sequencial: %lf\n", delta);
}
