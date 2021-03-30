#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS 2 //total de threads a serem criadas
#define ARRAY_SIZE 10000 //tamanho do array

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
   int idThread, nThreads;
   int* array;
} t_Args;

//funcao executada pelas threads
void *incrementArrayElements (void *arg) {
  t_Args *args = (t_Args *) arg;
  int i;
  for (i = args->idThread; i < ARRAY_SIZE; i += args->nThreads) {
    args->array[i]++;
  }
  printf("Sou a thread %d de %d threads\n", args->idThread, args->nThreads);
  free(arg); //aqui pode liberar a alocacao feita na main

  pthread_exit(NULL);
}

//funcao principal do programa
int main() {
  pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
  int array[ARRAY_SIZE], i, thread;
  for(i = 0; i < ARRAY_SIZE; i++) {
    array[i] = 0; //inicializa elementos com zero
  }

  t_Args *arg; //receberá os argumentos para a thread

  for(thread = 0; thread < NTHREADS; thread++) {
    arg = malloc(sizeof(t_Args));
    if (arg == NULL) {
      printf("--ERRO: malloc()\n"); exit(-1);
    }
    arg->idThread = thread; 
    arg->nThreads = NTHREADS;
    arg->array = array; 
    
    printf("--Cria a thread %d\n", thread);
    if (pthread_create(&tid_sistema[thread], NULL, incrementArrayElements, (void*) arg)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  //--espera todas as threads terminarem
  for (thread=0; thread<NTHREADS; thread++) {
    if (pthread_join(tid_sistema[thread], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
  }

  // printa array incrementado
  int resultIsCorrect = 1;
  for(i = 0; i < ARRAY_SIZE; i++) {
    if(array[i] != 1) {
      resultIsCorrect = 0;
      break;
    }
  }

  if(resultIsCorrect){
    printf("Resultado correto!\n");
  } else {
    printf("Resultado incorreto!\n");
  }

  printf("--Thread principal terminou\n");
  pthread_exit(NULL);

}