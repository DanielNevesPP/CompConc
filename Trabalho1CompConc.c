#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TAM_VETOR 10000
#define NTHREADS  2

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct{

    int idThread, tamVetor, vetorAux[TAM_VETOR];

} t_Args;

//funcao executada pelas threads
void *QuadradoVetor (void *arg){
    t_Args *args = (t_Args *) arg;

    for(int i = args->idThread; i < args->tamVetor; i += 3){
        args->vetorAux[i] *= args->vetorAux[i];
    }

    free(arg);
    pthread_exit(NULL);
}

//funcao principal do programa
int main(void){

    pthread_t tid_sistema[NTHREADS];
    int vetor[TAM_VETOR];
    int thread;
    t_Args *arg;

    //dando valores ao vetor
    for(int i = 0; i < TAM_VETOR; i++){
        vetor[i] = i;
        arg->vetorAux[i] = vetor[i];
    }

    //funcao da thread principal
    for(int i = 0; i < TAM_VETOR; i += 3){
        vetor[i] *= vetor[i];
    }

    for(thread = 0; thread < NTHREADS; thread++){
        //aloca e preenche argumentos para thread
        arg = malloc(sizeof(t_Args));
        if (arg == NULL) {
            printf("--ERRO: malloc()\n");
            exit(-1);
        }
        arg->idThread = thread;
        arg->tamVetor = TAM_VETOR;

        //cria as threads
        if(pthread_create(&tid_sistema[thread], NULL, QuadradoVetor, (void*)arg)){
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }

    //passa os valores obtidos no vetor da struct pro vetor principal
    for(int i = 0; i < TAM_VETOR; i++){
        vetor[i] = arg->vetorAux[i];
    }

    //espera todas as threads terminarem
    for (thread = 0; thread < NTHREADS; thread++) {
        if (pthread_join(tid_sistema[thread], NULL)) {
            printf("--ERRO: pthread_join() \n");
            exit(-1);
        }
  }

    //imprime o vetor final
/*    for(int i = 0; i < TAM_VETOR; i++){
        printf("%d\n", vetor[i]);
    }*/

    pthread_exit(NULL);
}
