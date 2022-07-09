#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t em_e, em_l, escr, leit; //semaforos
int e = 0, l = 0; //globais
int num_e = 2, num_l = 4;

void* leitores(void* l_id){
    while(1){
        printf("Leitor (%d) quer ler\n", l_id);
        sem_wait(&leit);
        sem_wait(&em_l); 
        l++;
        if(l==1) {
            sem_wait(&escr);
        }
        sem_post(&em_l);
        sem_post(&leit)
        printf("Leitor (%d) esta lendo\n", l_id); //le...
        sem_wait(&em_l); 
        l--;
        if(l==0){
            sem_post(&escr);
        }
        sem_post(&em_l);
    }
}

void* escritores(void* e_id){
    while(1){
        sem_wait(&em_e); 
        e++;
        if(e==1) sem_wait(&leit);
        sem_post(&em_e);
        printf("Escritor (%d) quer escrever\n", e_id);
        sem_wait(&escr);
        printf("Escritor (%d) esta escrevendo\n", e_id);
        sem_post(&escr);
        sem_wait(&em_e); 
        e--;
        if(e==0) sem_post(&leit);
        sem_post(&em_e);
    }
}

int main(){

    pthread_t *tid_e, *tid_l;

    for(int i = 0; i < num_e; i++){
        if(pthread_create(&tid_e, NULL, escritores, (void*) i)){
            printf("ERRO--pthread _create\n");
            return 3;
        }
    }

    for(int i = 0; i < num_l; i++){
        if(pthread_create(&tid_l, NULL, leitores, (void*) i)){
            printf("ERRO--pthread _create\n");
            return 3;
        }
    }

}