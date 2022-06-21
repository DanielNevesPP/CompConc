import java.util.Random;

//classe que estende a classe Thread
class Pares extends Thread{

    private int id;
    private int[] vetor;
    public static int qtd = 0;

    //construtor
    public Pares(int id, int[] vetor){
        this.id = id;
        this.vetor = vetor;
    }

    //metodo executado pela thread
    public synchronized void run(){
        for(int i = id; i < ParesVetor.tam; i += ParesVetor.N){
            if(vetor[i] % 2 == 0){
                qtd++;
            }
        }
    }
}

//classe do metodo main
class ParesVetor{

    public static final int N = 4; // numero de threads
    static final int tam = 1000; //tamanho do vetor

    public static void main (String[] args){

            Thread[] threads = new Thread[N];
            int[] vetor = new int[tam];
            int i, qtd = 0;

            //da valores aleatorios ao vetor de 0 a tam-1
            Random gerador = new Random();
            for(i = 0; i < tam; i++){
                vetor[i] = gerador.nextInt(tam); 
            }

            //ve quantos pares tem no vetor sequencialmente
            for(i = 0; i < tam; i++){
                if(vetor[i] % 2 == 0){
                    qtd++;
                }
            }

            //cria threads da classe que estende Thread
            for(i = 0; i < threads.length; i++){
                threads[i] = new Thread(new Pares(i, vetor));
            }

            //inicializa as threads
            for(i = 0; i < threads.length; i++){
                threads[i].start();
            }

            //espera pelo termino das threads
            for(i = 0; i < threads.length; i++){
                try{ 
                    threads[i].join();
                }
                catch(InterruptedException e)
                {
                    return;}
            }

            //printa o vetor
            /*for(i = 0; i < tam; i++){
                System.out.printf("%d\n", vetor[i]);
            }*/

            if(Pares.qtd == qtd){
                System.out.printf("Existem %d numeros pares no vetor\n", qtd);
            }

    }

}
