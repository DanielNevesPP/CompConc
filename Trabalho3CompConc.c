#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//#include "timer.h"

float *vetor;
long long int dim;
int nthreads;
float menorNum, maiorNum;

typedef struct{

    int id;

} tArgs;

void * MaiorMenor(void * arg){

    tArgs *args = (tArgs*) arg;
    long int tamBloco = dim / nthreads;
    long int ini = args->id * args->tamBloco;
    long int fim;
    if(args->id == nthreads - 1){
        fim = dim;
    }
    else{
        fim = ini + tamBloco;
    }

    for(int i = ini; i < fim; i++){

    }

}

int main(int argc, char *argv[]){

    if(argc < 3){
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero de threads>\n", argv[0]);
        return 1;
    }
    dim = atoll(argv[1]);
    nthreads = atoi(argv[2]);

    long int i;
    double inicio, fim, delta;
    pthread_t *tid;
    tArgs *args;

    vetor = (float*) malloc(sizeof(float)*dim);
    if(vetor == NULL){
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }

    //dando valores aleatorios de 0 a 1000 as matrizes
    srand(time(NULL));
    for(i = 0; i < dim; i++){
        vetor[i] = rand() % 1000;
    }

    //GET_TIME(inicio);
    //procura o maior e menor valores sequencialmente
    maiorNum = vetor[0];
    menorNum = vetor[0];
    for(i = 1; i < dim; i++){
        if(vetor[i] < menorNum){
            menorNum = vetor[i];
        }
        if(vetor[i] > maiorNum){
            maiorNum = vetor[i];
        }
    }
    //GET_TIME(fim);
    //delta = fim - inicio;

    //procura o maior e menor valores concorrentemente
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(tid == NULL){
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
    }

    for(i = 0; i < dim; i++){
        printf("%f\n", vetor[i]);
    }
    printf("menor: %f\n", menorNum);
    printf("maior: %f\n", maiorNum);

    return 0;
}
