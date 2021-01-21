#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int shared_arr[] = { 0, 0, 0, 0, 0 };
int read_count = 0;
sem_t w_sem;
sem_t r_sem;

typedef struct
{
	int id;
}
tArgs;

void *reader(void *arg)
{
	tArgs *args = (tArgs*) arg;
	while (1)
	{
		sem_wait(&r_sem);
		read_count++;
		if (read_count == 1)
		{
			sem_wait(&w_sem);
		}
		sem_post(&r_sem);
		printf("Reading - thread %d: shared_arr = {%d, %d, %d, %d, %d}\n", args->id, shared_arr[0], shared_arr[1], shared_arr[2], shared_arr[3], shared_arr[4]);
		usleep(3);
		sem_wait(&r_sem);
		read_count--;
		if (read_count == 0)
		{
			sem_post(&w_sem);
		}
		sem_post(&r_sem);
	}
}

void *writer(void *arg)
{
	tArgs *args = (tArgs*) arg;
	while (1)
	{
		sem_wait(&w_sem);
		printf("Writing - thread %d\n", (args->id));
		shared_arr[0] = (args->id) *2;
		shared_arr[1] = (args->id);
		shared_arr[2] = (args->id);
		shared_arr[3] = (args->id);
		shared_arr[4] = (args->id) *2;
		sem_post(&w_sem);
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

	pthread_t * tid;
	tArgs * args;

	sem_init(&w_sem, 1, 1);
	sem_init(&r_sem, 1, 1);

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

	sem_destroy(&w_sem);
	sem_destroy(&r_sem);

	return 0;
}