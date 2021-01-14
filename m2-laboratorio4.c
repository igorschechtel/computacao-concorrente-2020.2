/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 4 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */

/*
1. Implemente um programa com 4 threads. A thread 1 imprime a frase “tudo bem?”
A thread 2 imprime a frase “bom dia!” A thread 3 imprime a frase “ate mais!”. A
thread 4 imprime a frase “boa tarde!”.

2. As threads 1 e 2 devem executar antes das threads 3 e 4 sempre (a ordem de
execução entre as threads 1 e 2 não importa, assim como a ordem de execução
entre as threads 3 e 4).
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Thread A */
void *A (void *t) {
  printf("tudo bem?\n");
  pthread_mutex_lock(&x_mutex);
  x++;
  if (x==2) {
    pthread_cond_broadcast(&x_cond);
  }
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

/* Thread B */
void *B (void *t) {
  printf("bom dia!\n");
  pthread_mutex_lock(&x_mutex);
  x++;
  if (x==2) {
    pthread_cond_broadcast(&x_cond);
  }
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

/* Thread C */
void *C (void *t) {
  pthread_mutex_lock(&x_mutex);
  if (x < 2) { 
     pthread_cond_wait(&x_cond, &x_mutex);
  }
  printf("até mais!\n");
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

/* Thread D */
void *D (void *t) {
  pthread_mutex_lock(&x_mutex);
  if (x < 2) { 
     pthread_cond_wait(&x_cond, &x_mutex);
  }
  printf("boa tarde!\n");
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  /* Cria as threads */
  pthread_create(&threads[2], NULL, C, NULL);
  pthread_create(&threads[3], NULL, D, NULL);
  pthread_create(&threads[0], NULL, A, NULL);
  pthread_create(&threads[1], NULL, B, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("\nFIM\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
}
