import java.util.Random;

class Pares extends Thread{

    private int id;
    private int[] vetor;
    private int qtd = 0;

    public Pares(int idThread, int[] vetor){
        this.id = idThread;
        this.vetor = vetor;
    }

    public void run(){
        for(int i = 0; i < vetor.length; i++){
            if(vetor[i] % 2){
                qtd++;
            }
        }
    }
}



class ParesVetor{

    static final int N = 4;// numero de threads
    static final int tam = 50; //tamanho do vetor

    public static void main (String[] args){

            Thread[] threads = new Thread[N];
            int vetor = new int[tam];
            int i;

            Random gerador = new Random();
            for(i = 0; i < tam; i++){
                vetor[i] = gerador.nextInt(tam); //da valores aleatorios ao vetor de 0 a tam-1
            }

            for(i = 0; i < threads.length; i++){

            }

    }

}
