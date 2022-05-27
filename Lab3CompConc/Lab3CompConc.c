#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float *vetor;
long long int dim;
int nthreads;

void * maiorMenor(void * arg){

    long long int id = (long long int) arg; //identificador da thread
    float *valores; //vetor local com 2 espaços pra float para armazenar o menor e maior numero, respectivamente, do vetor principal
    valores = (float*) malloc(sizeof(float) * 2);
    if(valores == NULL){
        exit(1);
    }

    valores[0] = vetor[id]; //inicializa o menor valor
    valores[1] = vetor[id]; //inicializa o maior valor
    //calcula o menor e maior valores vistos pela thread
    for(long long int i = id +1; i < dim; i += nthreads + 1){
        if(vetor[i] < valores[0]){
            valores[0] = vetor[i];
        }
        if(vetor[i] > valores[1]){
            valores[1] = vetor[i];
        }
    }

    pthread_exit((void *) valores); //retorna os valores encontrados

}

//fluxo principal
int main(int argc, char *argv[]){

    //recebe os parametros de entrada
    if(argc < 3){
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero de threads>\n", argv[0]);
        return 1;
    }
    dim = atoll(argv[1]);
    nthreads = atoi(argv[2]);

    long long int i; //variavel auxiliar
    double inicio, fim, delta; //variaveis de tempo
    pthread_t *tid; //identificadores das threads no sistema
    float menorNumSeq, maiorNumSeq; //menor e maior valores encontrados sequencialmente
    float *retorno; //valor de retorno das threads

    //aloca o vetor de entrada
    vetor = (float*) malloc(sizeof(float)*dim);
    if(vetor == NULL){
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }

    //dando valores aleatorios de 0 a 100000 ao vetor
    srand(time(NULL));
    for(i = 0; i < dim; i++){
        vetor[i] = rand() % 100000;
    }

    //procura o maior e menor valores sequencialmente
    GET_TIME(inicio);
    maiorNumSeq = vetor[0];
    menorNumSeq = vetor[0];
    for(i = 1; i < dim; i++){
        if(vetor[i] < menorNumSeq){
            menorNumSeq = vetor[i];
        }
        if(vetor[i] > maiorNumSeq){
            maiorNumSeq = vetor[i];
        }
    }
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo sequencial:  %lf\n", delta);

    //procura o maior e menor valores concorrentemente
    GET_TIME(inicio);
    float menorConc = vetor[0], maiorConc = vetor[0]; //menor e maior valores encontrados concorrentemente
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(tid == NULL){
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
    }
    //thread principal
    for(i = 0; i < dim; i += nthreads + 1){
        if(vetor[i] < menorConc){
            menorConc = vetor[i];
        }
        if(vetor[i] > maiorConc){
            maiorConc = vetor[i];
        }
    }
    //cria as threads
    for(i = 0; i < nthreads; i++){
        if(pthread_create(tid + i, NULL, maiorMenor, (void*) i)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }
    //espera pelo termino das threads
    for(i = 0; i < nthreads; i++){
        if(pthread_join(*(tid+i), (void**) &retorno)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
        if(retorno[0] < menorConc){
            menorConc = retorno[0];
        }
        if(retorno[1] > maiorConc){
            maiorConc = retorno[1];
        }

    }
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo concorrente:  %lf\n\n", delta);

    //printa o vetor
    /*for(i = 0; i < dim; i++){
        printf("%f\n", vetor[i]);
    }*/

    //printa os menores e maiores valores encontrados
    /*printf("Menor valor sequencial: %f\n", menorNumSeq);
    printf("Maior valor sequencial: %f\n\n", maiorNumSeq);
    printf("Menor valor concorrente: %f\n", menorConc);
    printf("Maior valor concorrente: %f\n", maiorConc);*/

    //libera as areas de memoria alocadas
    free(tid);
    free(vetor);

    return 0;
}
