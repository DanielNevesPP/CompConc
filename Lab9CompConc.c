#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 5

int x = 0; //variavel global compartilhada entre as threads
sem_t cond1, cond2; //semaforos para sincronizar a ordem de execucao das threads

void *thread1(){
    sem_wait(&cond2);
    printf("Volte sempre!\n");

    pthread_exit(NULL);
}

void *thread2(){
    sem_wait(&cond1);
    printf("Fique a vontade.\n");
    x++;
    if(x == 3){
        sem_post(&cond2);
    }
    sem_post(&cond1);

    pthread_exit(NULL);
}

void *thread3(){
    sem_wait(&cond1);
    printf("Sente-se por favor.\n");
    x++;
    if(x == 3){
        sem_post(&cond2);
    }
    sem_post(&cond1);

    pthread_exit(NULL);
}

void *thread4(){
    sem_wait(&cond1);
    printf("Aceita um copo d'agua?\n");
    x++;
    if(x == 3){
        sem_post(&cond2);
    }
    sem_post(&cond1);

    pthread_exit(NULL);
}

void *thread5(){
    printf("Seja bem-vindo!\n");
    sem_post(&cond1);

    pthread_exit(NULL);
}

int main(){

    int i;
    pthread_t threads[NTHREADS];
    //inicializa os semaforos
    sem_init(&cond1, 0, 0);
    sem_init(&cond2, 0, 0);

    //cria as threads
    if(pthread_create(&threads[0], NULL, thread1, NULL)){ printf("--ERRO: pthread_create()\n"); exit(-1); };
    if(pthread_create(&threads[1], NULL, thread2, NULL)){ printf("--ERRO: pthread_create()\n"); exit(-1); };
    if(pthread_create(&threads[2], NULL, thread3, NULL)){ printf("--ERRO: pthread_create()\n"); exit(-1); };
    if(pthread_create(&threads[3], NULL, thread4, NULL)){ printf("--ERRO: pthread_create()\n"); exit(-1); };
    if(pthread_create(&threads[4], NULL, thread5, NULL)){ printf("--ERRO: pthread_create()\n"); exit(-1); };

    //espera as threads terminarem
    for(i = 0; i < NTHREADS; i++){
        if(pthread_join(threads[i], NULL)){
            printf("--ERRO: pthread_join() \n"); 
            exit(-1); 
        }
    }

    //desaloca as variaveis
    sem_destroy(&cond1);
    sem_destroy(&cond2);

    return 0;
}
