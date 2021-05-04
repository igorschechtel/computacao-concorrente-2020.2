#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>

pthread_barrier_t read_barrier;
pthread_barrier_t write_barrier;
int* array;
int n, p;

void* thread_task(void* tid);

int main(int argc, char *argv[]) {
    if (argc < 2) {
      printf("Digite: %s <p>\n(onde o número de elementos do vetor é 2^p)\n", argv[0]);
      return 1;
    }

    p = atoi(argv[1]);
    n = pow(2, p);

    array = (int*) malloc(n * sizeof(int));

    // inicializa as barreiras
    pthread_barrier_init (&read_barrier, NULL, n);
    pthread_barrier_init (&write_barrier, NULL, n);

    // inicializa o vetor
    for (int i = 0; i < n; i++) array[i] = i+1;

    // cria threads
    pthread_t threads[n];
    for (long i = 0; i < n; i++) {
      if (pthread_create(&threads[i], NULL, thread_task, (void*) i)) {
        printf("--ERRO: pthread_create()\n");
        exit(-1);
      }
    }

    // espera as threads terminarem
    for (long i = 0; i < n; i++) {
      if (pthread_join(threads[i], NULL)) {
        printf("--ERRO: pthread_join()\n");
        exit(-1); 
      }
    }

    // verifica se algoritmo esta correto
    int is_correct = 1;
    int prefix_sum = 0;
    for (int i = 0; i < n; i++) {
      prefix_sum += i+1;
      if(array[i] != prefix_sum) {
        is_correct = 0;
        break;
      }
    }
    
    if (is_correct) printf("Resultados corretos!\n");
    else printf("Resultados incorretos.\n");

    free(array);

    return 0;
}

void* thread_task (void* tid) {
    int thread_id = (long) tid;
    for(int i = 0; i < p; i++) {
      int salto = pow(2, i);
      int aux = 0;
      if (salto <= thread_id) {
        aux = array[thread_id - salto];
      }
      pthread_barrier_wait(&read_barrier);
      array[thread_id] += aux;
      pthread_barrier_wait(&write_barrier);
    }
}