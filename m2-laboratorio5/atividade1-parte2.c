#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int shared_arr[] = { 0, 0, 0, 0, 0 };
int read_count = 0, write_count = 0;
sem_t r_sem, w_sem;
sem_t x, y, z;

typedef struct
{
	int id;	// identificador de id da thread
}
tArgs;

void *reader(void *arg)
{
	tArgs *args = (tArgs*) arg;
	while (1)
	{
		sem_wait(&z);
		sem_wait(&r_sem);
		sem_wait(&x);
		read_count++;
		if (read_count == 1)
			sem_wait(&w_sem);
		sem_post(&x);
		sem_post(&r_sem);
		sem_post(&z);
		printf("Reading - thread %d: shared_arr = {%d, %d, %d, %d, %d}\n", args->id, shared_arr[0], shared_arr[1], shared_arr[2], shared_arr[3], shared_arr[4]);
		sem_wait(&x);
		read_count--;
		if (read_count == 0)
			sem_post(&w_sem);
		sem_post(&x);
	}
}

void *writer(void *arg)
{
	tArgs *args = (tArgs*) arg;
	while (1)
	{
		sem_wait(&y);
		write_count++;
		if (write_count == 1)
			sem_wait(&r_sem);
		sem_post(&y);
		sem_wait(&w_sem);
		printf("Writing - thread %d\n", (args->id));
		shared_arr[0] = (args->id) *2;
		shared_arr[1] = (args->id);
		shared_arr[2] = (args->id);
		shared_arr[3] = (args->id);
		shared_arr[4] = (args->id) *2;
		sem_post(&w_sem);
		sem_wait(&y);
		write_count--;
		if (write_count == 0)
			sem_post(&r_sem);
		usleep(3);
		sem_post(&y);
	}
}

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("Digite: %s<número de threads leitoras><número de threads escritoras>\n", argv[0]);
		return 1;
	}
	int N = atoi(argv[1]);
	int M = atoi(argv[2]);

	pthread_t * tid;	// identificadores das threads no sistema
	tArgs * args;	// identificadores locais das threads e dimensão

	sem_init(&r_sem, 1, 1);
	sem_init(&w_sem, 1, 1);
	sem_init(&x, 1, 1);
	sem_init(&y, 1, 1);
	sem_init(&z, 1, 1);

	// alocação das estruturas
	tid = (pthread_t*) malloc(sizeof(pthread_t) *(N + M));
	if (tid == NULL)
	{
		printf("ERRO--malloc\n");
		return 2;
	}
	args = (tArgs*) malloc(sizeof(tArgs) *(N + M));
	if (args == NULL)
	{
		printf("ERRO--malloc\n");
		return 2;
	}

	int i;
	for (i = 0; i < N; i++)
	{
		(args + i)->id = i;
		pthread_create(tid + i, NULL, reader, (void*)(args + i));
	}
	for (i = N; i < N + M; i++)
	{
		(args + i)->id = i;
		pthread_create(tid + i, NULL, writer, (void*)(args + i));
	}

	for (i = 0; i < N + M; i++)
	{
		pthread_join(*(tid + i), NULL);
	}

	sem_destroy(&r_sem);
	sem_destroy(&w_sem);
	sem_destroy(&x);
	sem_destroy(&y);
	sem_destroy(&z);

	return 0;
}