#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 5

int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond, y_cond;

void *thread1(){
    pthread_mutex_lock(&x_mutex);
    //imprime a mensagem apenas depois de imprimir as outras 4
    while(x < 4){
        pthread_cond_wait(&y_cond, &x_mutex); //espera o sinal da quarta thread a ser executada
    }
    pthread_mutex_unlock(&x_mutex);
    printf("Volte sempre!\n");

    pthread_exit(NULL);
}

void *thread2(){
    pthread_mutex_lock(&x_mutex);
    //imprime a mensagem apenas depois da primeira ser impressa
    if(x == 0){
        pthread_cond_wait(&x_cond, &x_mutex); //espera a primeira thread mandar o sinal
    }
    x++;
    printf("Fique a vontade.\n");
    //manda o sinal para a quinta thread ser imprimida caso essa seja a quarta thread
    if(x == 4){
        pthread_cond_signal(&y_cond);
    }
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}

void *thread3(){
    pthread_mutex_lock(&x_mutex);
    //imprime a mensagem apenas depois da primeira ser impressa
    if(x == 0){
        pthread_cond_wait(&x_cond, &x_mutex); //espera a primeira thread mandar o sinal
    }
    x++;
    printf("Sente-se por favor.\n");
    //manda o sinal para a quinta thread ser imprimida caso essa seja a quarta thread
    if(x == 4){
        pthread_cond_signal(&y_cond);
    }
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}

void *thread4(){
    pthread_mutex_lock(&x_mutex);
    //imprime a mensagem apenas depois da primeira ser impressa
    if(x == 0){
        pthread_cond_wait(&x_cond, &x_mutex); //espera a primeira thread mandar o sinal
    }
    x++;
    printf("Aceita um copo d'agua?\n");
    //manda o sinal para a quinta thread ser imprimida caso essa seja a quarta thread
    if(x == 4){
        pthread_cond_signal(&y_cond);
    }
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}

void *thread5(){
    printf("Seja bem-vindo!\n");
    pthread_mutex_lock(&x_mutex);
    x++;
    pthread_cond_broadcast(&x_cond); //manda o sinal para poder imprimir as outras threads
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}

int main(){

    int i;
    pthread_t threads[NTHREADS];
    //inicializa o mutex e as variaveis de condicao
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&x_cond, NULL);
    pthread_cond_init(&y_cond, NULL);

    pthread_create(&threads[0], NULL, thread1, NULL);
    pthread_create(&threads[1], NULL, thread2, NULL);
    pthread_create(&threads[2], NULL, thread3, NULL);
    pthread_create(&threads[3], NULL, thread4, NULL);
    pthread_create(&threads[4], NULL, thread5, NULL);

    for(i = 0; i < NTHREADS; i++){
        pthread_join(threads[i], NULL);
    }

    //desaloca as variaveis
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
    pthread_cond_destroy(&y_cond);

    return 0;
}
