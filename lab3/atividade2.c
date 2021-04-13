#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <assert.h>
#include "timer.h"

int T; // número de threads

typedef struct {
  int id; // identificador do elemento que a thread irá processar
  int N; // número de elementos da série
} tArgs;

void *tarefa (void *arg) {
  tArgs *args = (tArgs*) arg;
  double serie = 0;
  for (int i = (args->id); i < (args->N); i += T) {
    double denominador = 1 + (2*i);
    int subtrair = i%2;

    if(subtrair) serie -= (1/denominador);
    else serie += (1/denominador);
  }
  // printf("Thread %d\nSerie = %.5lf\n\n", args->id, serie);

  double* ptr = malloc(sizeof(*ptr));
  assert(ptr);
  *ptr = serie;
  pthread_exit(ptr);
}

int main(int argc, char* argv[]) {
  long long int N; // número de elementos da série
  pthread_t *tid; // identificadores das threads no sistema
  tArgs *args; // identificadores locais das threads e dimensão
  double inicio, fim, delta;

  if(argc < 3) {
    printf("Digite: %s <número de elementos da série> <número de threads>\n", argv[0]);
    return 1;
  }
  N = atoll(argv[1]);
  T = atoi(argv[2]);

  GET_TIME(inicio);

  // alocação das estruturas
  tid = (pthread_t*) malloc(sizeof(pthread_t) * T);
  if (tid == NULL) {
    printf("ERRO--malloc\n");
    return 2;
  }
  args = (tArgs*) malloc(sizeof(tArgs) * T);
  if (args == NULL) {
    printf("ERRO--malloc\n");
    return 2;
  }

  // criação das threads
  for (int i = 0; i < T; i++) {
    (args + i)->id = i;
    (args + i)->N = N;
    if(pthread_create(tid + i, NULL, tarefa, (void*) (args + i))) {
      puts("ERRO--pthread_create\n");
      return 3;
    }
  }
  
  // espera pelo término das threads
  double serie_thread[T];
  void* ptr;
  for (int i = 0; i < T; i++) {
    pthread_join(*(tid+i), &ptr);
    serie_thread[i] = *(double*)ptr;
    free(ptr);
  }

  // soma valores calculados pelas threads
  double soma_series = 0;
  for (int i = 0; i < T; i++) {
    soma_series += serie_thread[i];
  }
  double pi = soma_series * 4;

  GET_TIME(fim);

  delta = fim - inicio;

  printf("Pi real:      %.15f\n", M_PI);
  printf("Pi calculado: %.15f\n\n", pi);
  printf("Tempo de execução: %.2f segundos\n", delta);

  return 0;
}