#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float *matriz1;
float *matriz2;
float *matrizsaidaseq;
float *matrizsaidaconc;
int nthreads;

typedef struct{

    int dim, id;

} tArgs;

//funcao das threads
void *MultiplicaMatriz (void *arg){

    tArgs *args = (tArgs*) arg;

    for(int i = args->id + 1; i < args->dim; i += nthreads + 1){
        for(int j = 0; j< args->dim; j++){
            for(int k = 0; k < args->dim; k++){
                matrizsaidaconc[i * args->dim + j] += matriz1[i * args->dim + k] * matriz2[k * args->dim + j];
            }
        }
    }
    pthread_exit(NULL);

}

//fluxo principal
int main(int argc, char* argv[]){

    int dim, i, j, k;
    double inicio, fim, delta;
    pthread_t *tid;
    tArgs *args;
    clock_t t;

    //leitura e avaliacao dos parametros de entrada
    if(argc  < 3){
      printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
      return 1;
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    if (nthreads > dim){
      nthreads = dim;
    }

   //alocacao de memoria para as estruturas de dados
   matriz1 = (float *) malloc(sizeof(float) * dim * dim);
   if (matriz1 == NULL) {printf("ERRO--malloc\n"); return 2;}
   matriz2 = (float *) malloc(sizeof(float) * dim * dim);
   if (matriz2 == NULL) {printf("ERRO--malloc\n"); return 2;}
   matrizsaidaseq = (float *) malloc(sizeof(float) * dim * dim);
   if (matrizsaidaseq == NULL) {printf("ERRO--malloc\n"); return 2;}
   matrizsaidaconc = (float *) malloc(sizeof(float) * dim * dim);
   if (matrizsaidaconc == NULL) {printf("ERRO--malloc\n"); return 2;}

    //dando valores aleatorios as matrizes
    srand(time(NULL));
    for(i = 0; i < dim; i++){
      for(j = 0; j< dim; j++){
        matriz1[i * dim + j] = rand() % 10;
        matriz2[i * dim + j] = rand() % 10;
      }
    }

    //inicializando as matrizes de saida
    for(i = 0; i < dim; i++){
      for(j = 0; j < dim; j++){
        matrizsaidaseq[i * dim + j] = 0;
        matrizsaidaconc[i * dim + j] = 0;
      }
    }

    GET_TIME(inicio);
    //multiplicacao de matriz sequencial
    for(i = 0; i < dim; i++){
      for(j = 0; j< dim; j++){
        for(k = 0; k < dim; k++){
          matrizsaidaseq[i * dim + j] += matriz1[i * dim + k] * matriz2[k * dim + j];
        }
      }
    }
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo sequencial: %lf\n", delta);

    //multiplicacao de matriz concorrente
    GET_TIME(inicio);
    //alocacao das estruturas
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL) {puts("ERRO--malloc"); return 2;}
    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
    if(args==NULL) {puts("ERRO--malloc"); return 2;}

    //thread principal
    for(i = 0; i < dim; i += nthreads + 1){
      for(j = 0; j< dim; j++){
        for(k = 0; k < dim; k++){
          matrizsaidaconc[i * dim + j] += matriz1[i * dim + k] * matriz2[k * dim + j];
        }
      }
    }

    //criando as threads
    for(int i = 0; i < nthreads; i++) {
      (args+i)->id = i;
      (args+i)->dim = dim;
      if(pthread_create(tid+i, NULL, MultiplicaMatriz, (void*) (args+i))){
         puts("ERRO--pthread_create"); return 3;
      }
    }

    //espera pelo termino da threads
    for(int i = 0; i < nthreads; i++) {
      pthread_join(*(tid+i), NULL);
    }
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo concorrente: %lf\n", delta);

    //compara as matrizes de saida
    for(i = 0; i < dim; i++){
        for(j = 0; j < dim; j++){
            if(matrizsaidaseq[i * dim + j] != matrizsaidaconc[i * dim + j]){
                puts("ERRO--matrizes diferentes");
                return 4;
            }
        }
    }

    //liberacao da memoria
    free(matriz1);
    free(matriz2);
    free(matrizsaidaseq);
    free(matrizsaidaconc);
    free(args);
    free(tid);

    return 0;
}
