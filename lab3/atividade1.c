#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

int main(int argc, char* argv[]) {
  long long int N;

  double inicio, fim, delta;

  if(argc < 2) {
    printf("Digite: %s <número de elementos da série>\n", argv[0]);
    return 1;
  }
  N = atoll(argv[1]);

  double serie = 0;

  GET_TIME(inicio);
  
  for(int i = 0; i < N; i++) {
    double denominador = 1 + (2*i);
    int subtrair = i%2;

    if(subtrair) serie -= (1/denominador);
    else serie += (1/denominador);
  }

  double pi = 4*serie;

  GET_TIME(fim);

  delta = fim - inicio;

  printf("Pi real:      %.15lf\n", M_PI);
  printf("Pi calculado: %.15lf\n\n", pi);

  printf("Tempo de execução: %.2lf segundos\n", delta);

  // Podemos observar que o valor calculado se aproxima mais
  // de PI quanto maior for o valor de N

  return 0;
}