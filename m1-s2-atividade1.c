#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float *mat; // matriz de entrada
float *vet; // vetor de entrada
float *saida; // vetor de saída
int nthreads; // número de threads

typedef struct {
  int id; // identificador do elemento que a thread irá processar
  int dim; // dimensão da estrutura de entrada
} tArgs;

// função que as threads executarão
void *tarefa (void *arg) {
  tArgs *args = (tArgs*) arg;
  // printf("Thread %d\n", args->id);
  for (int i = (args->id); i < (args->dim); i += nthreads) {
    for (int j = 0; j < (args->dim); j++) {
      saida[i] += mat[i*(args->dim) + j] * vet[j];
    }
  }
  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  int dim; // dimensão da matriz de entrada
  pthread_t *tid; // identificadores das threads no sistema
  tArgs *args; // identificadores locais das threads e dimensão
  double inicio, fim, delta;

  GET_TIME(inicio);
  // leitura e avaliação dos parâmetros de entrada
  if(argc < 3) {
    printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
    return 1;
  }
  dim = atoi(argv[1]);
  nthreads = atoi(argv[2]);
  if (nthreads > dim) nthreads = dim;

  // alocação de memória para as estruturas de dados
  mat = (float*) malloc(sizeof(float) * dim * dim);
  if (mat == NULL) {
    printf("ERRO--malloc\n");
    return 2;
  }
  
  vet = (float*) malloc(sizeof(float) * dim);
  if (vet == NULL) {
    printf("ERRO--malloc\n");
    return 2;
  }

  saida = (float*) malloc(sizeof(float) * dim);
  if (saida == NULL) {
    printf("ERRO--malloc\n");
    return 2;
  }

  // inicialização das estruturas de dados
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      mat[i*dim + j] = 1;
    }
    vet[i] = 1;
    saida[i] = 0;
  }
  GET_TIME(fim);
  delta = fim - inicio;
  // printf("Tempo inicialização: %lf\n", delta);

  GET_TIME(inicio);
  /* -------------------------------
  multiplicação da matriz pelo vetor
  ------------------------------- */

  // alocação das estruturas
  tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
  if (tid == NULL) {
    printf("ERRO--malloc\n");
    return 2;
  }
  args = (tArgs*) malloc(sizeof(tArgs) * nthreads);
  if (args == NULL) {
    printf("ERRO--malloc\n");
    return 2;
  }

  // criação das threads
  for (int i = 0; i < nthreads; i++) {
    (args + i)->id = i;
    (args + i)->dim = dim;
    if(pthread_create(tid + i, NULL, tarefa, (void*) (args + i))) {
      puts("ERRO--pthread_create\n");
      return 3;
    }
  }

  // espera pelo término das threads
  for (int i = 0; i < nthreads; i++) {
    pthread_join(*(tid+i), NULL);
  }
  GET_TIME(fim);
  delta = fim - inicio;
  printf("Tempo multiplicação: %lf\n", delta);


  // exibição dos resultados
  /*
  puts("Matriz de entrada:");
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      printf("%.1f ", mat[i*dim + j]);
    }
    puts("");
  }
  puts("Vetor de entrada:");
  for (int j = 0; j < dim; j++) {
    printf("%.1f ", vet[j]);
  }
  puts("");
  puts("Vetor de saída:");
  for (int j = 0; j < dim; j++) {
    printf("%.1f ", saida[j]);
  }
  puts("");
  */

  // liberação da memória
  GET_TIME(inicio);
  free(mat);
  free(vet);
  free(saida);
  free(args);
  free(tid);
  GET_TIME(fim);
  delta = fim - inicio;
  // printf("Tempo finalização: %lf\n", delta);

  return 0;
}