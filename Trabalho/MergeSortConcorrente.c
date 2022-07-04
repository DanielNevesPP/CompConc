#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

int *vetorSeq, *vetorConc, dim, N;

//funcao do mergesort
void mergeSort(int *vetor, int inicio, int fim){

    int meio;
    if(inicio < fim){
        meio = floor((inicio + fim) / 2); //calcula o meio do vetor
        mergeSort(vetor, inicio, meio); //chama a funcao recursivamente para a primeira metade do vetor
        mergeSort(vetor, meio + 1, fim); //chama a funcao recursivamente para a segunda metade do vetor
        merge(vetor, inicio, meio, fim); //combina as duas metades do vetor de forma ordenada
    }

}

//funcao para combinar as duas metades do vetor de forma ordenada
void merge(int *vetor, int inicio, int meio, int fim){

    int *temp, p1, p2, tam, i, j, k, fim1 = 0, fim2 = 0;

    tam = fim - inicio + 1;
    p1 = inicio; //p1 comeca no inicio do vetor
    p2 = meio + 1; //p2 comeca no meio do vetor
    temp = (int *) malloc(tam * sizeof(int)); //vetor temporario para auxiliar no programa

    if (temp != NULL){
        for(i = 0; i < tam; i++){
            //caso p1 nao tenha chegado ao meio ou p2 nao tenha chegado ao final do vetor
            if(!fim1 && !fim2){
                //caso o valor na posicao p1 do vetor seja menor que o da posicao p2, copia ele para a posicao i do vetor temporario e adiciona 1 a p1 (anda p1 para a proxima casa do vetor)
                if(vetor[p1] < vetor[p2]){
                    temp[i] = vetor[p1];
                    p1++;
                }
                //caso contrario, copia o valor da posicao p2 do vetor para a posicao i do vetor temporario e adiciona 1 a p2 (anda p2 para a proxima casa do vetor)
                else{
                    temp[i] = vetor[p2];
                    p2++;
                }
                if(p1 > meio){
                    fim1 = 1; //indica que p1 chegou ao meio do vetor
                }
                if(p2 > fim){
                    fim2 = 1; //indica que p2 chegou ao fim do vetor
                }
            }
            //caso p1 ja tenha chegado no meio ou p2 ja tenha chegado no fim do vetor
            else{
                //caso p2 ja tenha chegado no fim do vetor
                if(!fim1){
                    temp[i] = vetor[p1]; //copia o restante da primeira metade do vetor no vetor temporario
                    p1++;
                }
                else{
                    temp[i] = vetor[p2]; //copia o restante da segunda metade do vetor no vetor temporario
                    p2++;
                }
            }
        }
        //copia o vetor temporario no vetor principal
        for(j = 0, k = inicio; j < tam; j++, k++){
            vetor[k] = temp[j];
        }
    }
    free(temp); //libera o vetor temporario

}

void *mergeSortConc(void *arg){

    int id = (int) arg;
    int fim;
    int inicio = floor((dim / N)) * id;
    if(id != (N - 1)){
        fim = (floor((dim / N)) * (id + 1)) - 1;
    }
    else{
        fim = dim - 1;
    }
    mergeSort(vetorConc, inicio, fim);

}

//funcao principal
int main(int argc, char* argv[]){

    int i;
    pthread_t *tid; //identificadores das threads no sistema
    clock_t t; //variavel para armazenar tempo

    if(argc != 3){
        printf("Digite: %s (tamanho do vetor) (numero de threads)\n", argv[0]); //exemplo: ./ordena 1000 2 (mergesort de vetor de tamanho 1000 com 2 threads)
        return 1;
    }
    dim = atoi(argv[1]);
    N = atoi(argv[2]);

    vetorSeq = malloc(dim * sizeof(int));
    vetorConc = malloc(dim * sizeof(int));

    //da valores aleatorios ao vetor
    srand(time(NULL));
    for(i = 0; i < dim; i
    ++){
        vetorSeq[i] = rand() % dim;
        vetorConc[i] = vetorSeq[i];
    }

    t = clock();
    mergeSort(vetorSeq, 0, dim - 1); //faz o mergesort sequencialmente
    t = clock() - t;
    printf("Tempo sequencial: %lf\n", ((double)t)/(CLOCKS_PER_SEC));

    //parte concorrente
    t = clock();
    tid = (pthread_t *) malloc(sizeof(pthread_t) * N);
    if(tid == NULL){
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
    }

    for(i = 0; i < N; i++){
        if(pthread_create(&tid[i], NULL, mergeSortConc, (void*) i)){
            printf("ERRO--pthread_create\n");
            exit(-1);
        }
    }

    for(i = 0; i < N; i++){
        if(pthread_join(tid[i], NULL)){
            printf("ERRO--pthread_join\n");
            exit(-1);
        }
    }

    merge(vetorConc, 0, (dim-1)/2, dim-1);

    t = clock() - t;
    printf("Tempo concorrente: %lf\n", ((double)t)/(CLOCKS_PER_SEC));

    //printa o vetor
    /*for(i = 0; i < dim; i++){
        printf("%d\n", vetorSeq[i]);
        if(vetorSeq[i] != vetorConc[i]){
            printf("vetor errado\n");
            break;
        }
    }*/

    return 0;

}
