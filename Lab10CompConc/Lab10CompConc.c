#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t em_e, em_l, escr, leit; //semaforos
int e = 0, l = 0; //globais
int num_e = 2, num_l = 4;

void* leitores(void* threadid){

    int tid = *(int*) threadid;

    while(1){
        printf("Leitor (%d) quer ler\n", tid);
        sem_wait(&leit);
        sem_wait(&em_l); 
        l++;
        if(l==1) {
            sem_wait(&escr);
        }
        sem_post(&em_l);
        sem_post(&leit);
        printf("Leitor (%d) esta lendo\n", tid);
        sem_wait(&em_l); 
        l--;
        printf("Leitor (%d) terminou de ler\n", tid);
        if(l==0){
            sem_post(&escr);
        }
        sem_post(&em_l);
    }
    pthread_exit(NULL);
}

void* escritores(void* threadid){

    int tid = *(int*) threadid;

    while(1){
        printf("Escritor (%d) quer escrever\n", tid);
        sem_wait(&em_e); 
        e++;
        if(e==1){ 
            sem_wait(&leit);
        }
        sem_post(&em_e);
        sem_wait(&escr);
        printf("Escritor (%d) esta escrevendo\n", tid);
        sem_post(&escr);
        sem_wait(&em_e); 
        e--;
        if(e==0){ 
            sem_post(&leit);
        }
        printf("Escritor (%d) terminou de escrever\n", tid);
        sem_post(&em_e);
    }
    pthread_exit(NULL);
}

int main(){

    pthread_t tid_e[num_e], tid_l[num_l];
    int i;

    sem_init(&em_e, 0, 1);
    sem_init(&em_l, 0, 1);
    sem_init(&leit, 0, 1);
    sem_init(&escr, 0, 1);

    for(i = 0; i < num_e; i++){
        if(pthread_create(&tid_e[i], NULL, escritores, (void*) i)){
            printf("ERRO--pthread _create\n");
            return 3;
        }
    }

    for(i = 0; i < num_l; i++){
        if(pthread_create(&tid_l[i], NULL, leitores, (void*) i)){
            printf("ERRO--pthread _create\n");
            return 3;
        }
    }

    for(i = 0; i < num_e; i++){
        if (pthread_join(tid_e[i], NULL)) {
            printf("--ERRO: pthread_join() \n"); 
            exit(-1); 
        } 
    } 

    for(i = 0; i < num_l; i++){
        if (pthread_join(tid_l[i], NULL)) {
            printf("--ERRO: pthread_join() \n"); 
            exit(-1); 
        } 
    }

    pthread_exit(NULL);

}
