#include<stdio.h>
#include<stdlib.h>

float *mat; // matriz de entrada
float *vet; // vetor de entrada
float *saida; //vetor de saída

typedef struct {
  int id; // identificador do elemento que a thread irá processar
  int dim; // dimensão da estrutura de entrada
} tArgs;

// função que as threads executarão
void *tarefa (void *arg) {
  tArgs *args = (tArgs*) arg;
  for (int j = 0; j < (args->dim); j++) {
    saida[args->id] += mat[(args->id)*(args->dim) + j] * vet[j];
  }
  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  int dim; // dimensão da matriz de entrada
  pthread_t *tid; // identificadores das threads no sistema
  tArgs *args; // identificadores locais das threads e dimensão

  // leitura e avaliação dos parâmetros de entrada
  if(argc < 2) {
    printf("Digite: %s <dimensao da matriz>\n", argv[0]);
    return 1;
  }
  dim = atoi(argv[1]);

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

  /* -------------------------------
  multiplicação da matriz pelo vetor
  ------------------------------- */

  // alocação das estruturas
  tid = (pthread_t*) malloc(sizeof(pthread_t) * dim);
  if (tid == NULL) {
    printf("ERRO--malloc\n");
    return 2;
  }
  args = (tArgs*) malloc(sizeof(tArgs) * dim);
  if (args == NULL) {
    printf("ERRO--malloc\n");
    return 2;
  }

  // criação das threads
  for (int i = 0; i < dim; i++) {
    (args + i)->id = 1;
    if(pthread_create(tid + i, NULL, tarefa, (void*))) {
      puts("ERRO--pthread_create\n");
      return 3;
    }
  }

  // espera pelo término das threads
  for (int i = 0; i < dim; i++) {
    pthread_join(*(tid+i), NULL);
  }


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
  */
  puts("Vetor de saída:");
  for (int j = 0; j < dim; j++) {
    printf("%.1f ", saida[j]);
  }
  puts("");

  // liberação da memória
  free(mat);
  free(vet);
  free(saida);
  free(args);
  free(tid);

  return 0;
}