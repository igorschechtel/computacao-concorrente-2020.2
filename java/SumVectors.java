/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 6 */
/* Codigo: Acessando variável compartilhada em um programa multithreading Java */
/* -------------------------------------------------------------------*/

import java.util.Arrays;

// classe da estrutura de dados (recurso) compartilhado entre as threads
class Vector {
  private int size;
  private int[] vector;

  // constructor
  public Vector(int size) {
    this.size = size;
    this.vector = new int[size];
  }

  public void setElementByIndex(int index, int value) {
    this.vector[index] = value;
  }

  public int getElementByIndex(int index) {
    return this.vector[index];
  }

  public int getSize() {
    return this.size;
  }

  public String toString() {
    return Arrays.toString(this.vector);
  }
}

//classe que estende Thread e implementa a tarefa de cada thread do programa 
class T extends Thread {
   private int id;
   private Vector A, B, C;
   private int N_THREADS;
   private int ARRAYS_SIZE;
  
   //construtor
   public T(int threadId, Vector A, Vector B, Vector C, int N_THREADS) { 
      this.id = threadId;
      this.A = A;
      this.B = B;
      this.C = C;
      for(int i = 0; i < A.getSize(); i++) {
        this.A.setElementByIndex(i, 1);
        this.B.setElementByIndex(i, 2);
      }
      this.N_THREADS = N_THREADS;
      this.ARRAYS_SIZE = A.getSize();
   }

   //metodo main da thread
   public void run() {
      System.out.println("Thread " + this.id + " iniciou!");
      for (int i = this.id; i < this.ARRAYS_SIZE ; i += this.N_THREADS) {
          this.C.setElementByIndex(i, this.A.getElementByIndex(i) + this.B.getElementByIndex(i));
      }
      System.out.println("Thread " + this.id + " terminou!"); 
   }
}

//classe da aplicacao
class SumVectors {
  static final int ARRAYS_SIZE = 100;
  static final int N_THREADS = 4;

   public static void main (String[] args) {
      //reserva espaço para um vetor de threads
      Thread[] threads = new Thread[N_THREADS];

      //cria uma instancia do recurso compartilhado entre as threads
      Vector A = new Vector(ARRAYS_SIZE);
      Vector B = new Vector(ARRAYS_SIZE);
      Vector C = new Vector(ARRAYS_SIZE);

      //cria as threads da aplicacao
      for (int i = 0; i < N_THREADS; i++) {
         threads[i] = new T(i, A, B, C, N_THREADS);
      }

      //inicia as threads
      for (int i = 0; i < N_THREADS; i++) {
         threads[i].start();
      }

      //espera pelo termino de todas as threads
      for (int i = 0; i < N_THREADS; i++) {
         try { threads[i].join(); } catch (InterruptedException e) { return; }
      }

      System.out.println("A = " + A.toString());
      System.out.println("B = " + B.toString());
      System.out.println("C = " + C.toString());
   }
}
