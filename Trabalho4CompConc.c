#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

long long int dim;
int nthreads;
int *vetorEntrada;
float *vetorSaidaSeq;
float *vetorSaidaConc;
int global;
pthread_mutex_t mutex;

int ehPrimo(long long int n) {
    if (n<=1) return 0;
    if (n==2) return 1;
    if (n%2==0) return 0;
    for (int i=3; i<sqrt(n)+1; i+=2)
        if(n%i==0) return 0;
    return 1;
}

void processaPrimos(int vetorEntrada[], float vetorSaida[], int dim) {
    for(int i = 0; i < dim; i++) {
        if (ehPrimo(vetorEntrada[i]))
            vetorSaida[i] = sqrt(vetorEntrada[i]);
        else
            vetorSaida[i] = vetorEntrada[i];
    }
}

void *quadradoPrimos(){

    pthread_mutex_lock(&mutex);
    int i = global; 
    global++;
    pthread_mutex_unlock(&mutex);
    
    while(i < dim){
        if (ehPrimo(vetorEntrada[i]))
            vetorSaidaConc[i] = sqrt(vetorEntrada[i]);
        else
            vetorSaidaConc[i] = vetorEntrada[i];
        pthread_mutex_lock(&mutex);
        i = global;
        global++;
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char *argv[]){

    if(argc < 3){
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero de threads>\n", argv[0]);
        return 1;
    }
    dim = atoll(argv[1]);
    nthreads = atoi(argv[2]);

    int i;
    double inicio, fim, delta;
    pthread_t tid[NTHREADS];

    vetorEntrada = malloc(sizeof(int) * dim);
    vetorSaidaSeq = malloc(sizeof(int) * dim);
    vetorSaidaConc = malloc(sizeof(int) * dim);

    srand(time(NULL));
    for(i = 0; i < dim; i++){
        vetorEntrada[i] = rand() % dim;
    }
    GET_TIME(inicio);
    processaPrimos(vetorEntrada, vetorSaida, dim);
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo sequencial: %lf\n", delta);

    for(i = 0; i < nthreads; i++){
        if(pthread_create(&tid[t], NULL, ExecutaTarefa, NULL)
    }

    printf("Vetor de entrada:\n");
    for(i = 0; i < dim; i++){
        printf("%d\n", vetorEntrada[i]);
    }

    printf("Vetor de saida:\n");
    for(i = 0; i < dim; i++){
        printf("%f\n", vetorSaidaSeq[i]);
    }
}