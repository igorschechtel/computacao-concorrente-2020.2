/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 4 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */

/* Atividade 4 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t cond1, cond2;

/* Thread A */
void *A (void *t) {
  pthread_mutex_lock(&x_mutex);
  pthread_cond_wait(&cond2, &x_mutex);
  printf("Volte sempre!\n");
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

/* Thread B */
void *B (void *t) {
  pthread_mutex_lock(&x_mutex);
  pthread_cond_wait(&cond1, &x_mutex);
  printf("Fique a vontade.\n");
  x++;
  if (x==2) pthread_cond_signal(&cond2);
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

/* Thread C */
void *C (void *t) {
  pthread_mutex_lock(&x_mutex);
  pthread_cond_wait(&cond1, &x_mutex);
  printf("Sente-se por favor.\n");
  x++;
  if (x==2) pthread_cond_signal(&cond2);
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

/* Thread D */
void *D (void *t) {
  printf("Seja bem-vindo!\n");
  pthread_mutex_lock(&x_mutex);
  pthread_cond_broadcast(&cond1);
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e variaveis de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&cond1, NULL);
  pthread_cond_init (&cond2, NULL);

  /* Cria as threads */
  pthread_create(&threads[0], NULL, A, NULL);
  pthread_create(&threads[1], NULL, B, NULL);
  pthread_create(&threads[2], NULL, C, NULL);
  pthread_create(&threads[3], NULL, D, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&cond1);
  pthread_cond_destroy(&cond2);
}
