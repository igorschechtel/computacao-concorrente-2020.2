#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

// variáveis globais
int NUM_THREADS, ARRAY_LENGTH, AMOUNT_PER_THREAD, REMAINDERS;
int* array;

// funções
int getRandomNumber(int min, int max);
void* threadMergeSort(void* tid);
void mergeThreadArraySections(int* array, int amount, int section);
void checkArrayIsSorted(int* array);
void mergeSort(int* array, int start, int end);
void merge(int* array, int start, int middle, int end);

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Digite: %s <número de threads> <tamanho do vetor>\n", argv[0]);
    return 1;
  }

  NUM_THREADS = atoi(argv[1]);
  ARRAY_LENGTH = atoi(argv[2]);
  AMOUNT_PER_THREAD = ARRAY_LENGTH / NUM_THREADS;
  REMAINDERS = ARRAY_LENGTH % NUM_THREADS;
  array = (int*) malloc(ARRAY_LENGTH * sizeof(int));

  srand(time(NULL));
  double start, finish, elapsed;
  
  // inicializa o array com números aleatórios
  for (int i = 0; i < ARRAY_LENGTH; i++) {
    array[i] = getRandomNumber(1, 100000);
  }
  
  pthread_t threads[NUM_THREADS];
  GET_TIME(start);
  
  // cria as threads
  for (long i = 0; i < NUM_THREADS; i++) {
    if (pthread_create(&threads[i], NULL, threadMergeSort, (void*) i)) {
      printf("--ERRO: pthread_create()\n");
      exit(-1);
    }
  }
  
  // espera as threads terminarem
  for (long i = 0; i < NUM_THREADS; i++) {
    if (pthread_join(threads[i], NULL)) {
      printf("--ERRO: pthread_join()\n");
      exit(-1); 
    }
  }

  // junta ordenações locais obtidas pelas threads para ordernar o array como um todo
  mergeThreadArraySections(array, NUM_THREADS, 1);
  
  // calcula tempo de execução
  GET_TIME(finish);
  elapsed = finish - start;
  printf("Tempo de execução: %.2lf segundos\n", elapsed);
  
  checkArrayIsSorted(array);
  free(array);
  return 0;
}

// gera um número aleatório entre min e max
int getRandomNumber(int min, int max) {
  return min + (max - min) * ((double)rand() / RAND_MAX);
}

// função a ser executada pelas threads
void* threadMergeSort(void* tid) {
  int threadId = (long) tid;
  int start = threadId * AMOUNT_PER_THREAD;
  int end = (threadId + 1) * AMOUNT_PER_THREAD - 1;

  if (threadId == NUM_THREADS - 1) end += REMAINDERS;

  int middle = (start + end) / 2;
  
  if (start < end) {
    mergeSort(array, start, end);
    mergeSort(array, start + 1, end);
    merge(array, start, middle, end);
  }

  pthread_exit(NULL);
}

// junta os vetores ordenados localmente pelas threads para formar o array ordenado como um todo
void mergeThreadArraySections(int* array, int amount, int section) {
  for(int i = 0; i < amount; i += 2) {
    int start = i * (AMOUNT_PER_THREAD * section);
    int end = ((i + 2) * AMOUNT_PER_THREAD * section) - 1;
    int middle = start + (AMOUNT_PER_THREAD * section) - 1;
    if (end >= ARRAY_LENGTH) end = ARRAY_LENGTH - 1;
    merge(array, start, middle, end);
  }
  if (amount / 2 >= 1) mergeThreadArraySections(array, amount / 2, section * 2);
}

// verifica se o vetor foi ordenado corretamente
void checkArrayIsSorted(int* array) {
  for (int i = 1; i < ARRAY_LENGTH; i++) {
    if (array[i] < array[i - 1]) {
      printf("---ERRO: Vetor desordenado.");
      return;
    }
  }
  printf("Vetor ordenado com sucesso!\n");
}

// função sequencial de merge sort
void mergeSort(int* array, int start, int end) {
  if (start < end) {
    int middle = (end + start) / 2;
    mergeSort(array, start, middle);
    mergeSort(array, middle + 1, end);
    merge(array, start, middle, end);
  }
}

// função sequencial de merge
void merge(int* array, int start, int middle, int end) {
  int start1 = start, start2 = middle + 1, startAux = 0, size = end - start + 1;
  int* auxArray;
  auxArray = (int*) malloc(size * sizeof(int));

  while (start1 <= middle && start2 <= end) {
    if (array[start1] < array[start2]) {
      auxArray[startAux] = array[start1];
      start1++;
    } else {
      auxArray[startAux] = array[start2];
      start2++;
    }
    startAux++;
  }
  while (start1 <= middle) {
    auxArray[startAux] = array[start1];
    startAux++;
    start1++;
  }
  while (start2 <= end) {
    auxArray[startAux] = array[start2];
    startAux++;
    start2++;
  }
  for (startAux = start; startAux <= end; startAux++) {
    array[startAux] = auxArray[startAux - start];
  }
  free(auxArray);
}