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
pthread_mutex_t lock;

//funcao que verifica se o numero eh primo
int ehPrimo(long long int n) {
    if (n<=1) return 0;
    if (n==2) return 1;
    if (n%2==0) return 0;
    for (int i=3; i<sqrt(n)+1; i+=2)
        if(n%i==0) return 0;
    return 1;
}

//funcao para calcular a raiz quadrada dos primos do vetor
void processaPrimos(int vetorEntrada[], float vetorSaida[], int dim) {
    for(int i = 0; i < dim; i++) {
        if (ehPrimo(vetorEntrada[i]))
            vetorSaida[i] = sqrt(vetorEntrada[i]);
        else
            vetorSaida[i] = vetorEntrada[i];
    }
}

//funcao das threads
void *quadradoPrimos(){

    pthread_mutex_lock(&lock);
    int i = global;
    global++;
    pthread_mutex_unlock(&lock);

    while(i < dim){
        if (ehPrimo(vetorEntrada[i]))
            vetorSaidaConc[i] = sqrt(vetorEntrada[i]);
        else
            vetorSaidaConc[i] = vetorEntrada[i];
        pthread_mutex_lock(&lock);
        i = global;
        global++;
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);
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
    pthread_t tid[nthreads];
    pthread_mutex_init(&lock, NULL); //inicializa a variavel de exclusao mutua

    vetorEntrada = malloc(sizeof(int) * dim);
    vetorSaidaSeq = malloc(sizeof(int) * dim);
    vetorSaidaConc = malloc(sizeof(int) * dim);

    //inicializa o vetor de entrada
    for(i = 0; i < dim; i++){
        vetorEntrada[i] = 0;
    }
    //da valores aleatorios ao vetor de entrada
    srand(time(NULL));
    for(i = 0; i < dim; i++){
        for(int j = 0; j < 100; j++){
            vetorEntrada[i] += (rand() % 32767);
        }
    }

    //funcao sequencial
    GET_TIME(inicio);
    processaPrimos(vetorEntrada, vetorSaidaSeq, dim);
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo sequencial: %lf\n", delta);

    //parte concorrente
    GET_TIME(inicio);
    for(i = 0; i < nthreads; i++){
        if(pthread_create(&tid[i], NULL, quadradoPrimos, NULL)){
            printf("ERRO--pthread_create\n");
            exit(-1);
        }
    }

    for(i = 0; i < nthreads; i++){
        if(pthread_join(tid[i], NULL)){
            printf("ERRO--pthread_join\n");
            exit(-1);
        }
    }
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo concorrente: %lf\n", delta);

    //printa os vetores de entrada e saida
    /*printf("Vetor de entrada:\n");
    for(i = 0; i < dim; i++){
        printf("%d\n", vetorEntrada[i]);
    }
    printf("\nVetor de saida:\n");
    for(i = 0; i < dim; i++){
        printf("%f\n", vetorSaidaSeq[i]);
    }
    //compara o vetor de saida cncorrente com o sequencial
    for(i = 0; i < dim; i++){
        if(vetorSaidaConc[i] != vetorSaidaSeq[i]){
            printf("\nMatrizes diferentes\n");
        }
    }*/

    pthread_mutex_destroy(&lock);
    free(tid);
    free(vetorEntrada);
    free(vetorSaidaConc);
    free(vetorSaidaSeq);
}
