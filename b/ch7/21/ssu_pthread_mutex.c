#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *ssu_loop1(void *arg);
void *ssu_loop2(void *arg);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int shared_value;

int main(void)
{
	pthread_t tid1, tid2;
	int status;

	shared_value = 0;
	
	/*create new thread*/
	if (pthread_create(&tid1, NULL, ssu_loop1, NULL) != 0)
	{
		fprintf(stderr,  "pthread_create error\n");
		exit(1);
	}

	sleep(1);

	/*create new thread*/
	if (pthread_create(&tid2, NULL, ssu_loop2, NULL) != 0)
	{
		fprintf(stderr,  "pthread_create error\n");
		exit(1);
	}

	/*wait for thread1 to exit*/
	if (pthread_join(tid1, (void *)&status) != 0)
	{
		fprintf(stderr, "pthread_join error\n");
		exit(1);
	}

	/*wait for thread2 to exit*/
	if (pthread_join(tid2, (void *)&status) != 0)
	{
		fprintf(stderr, "pthread_join error\n");
		exit(1);
	}

	/*free the mutex variable*/
	status = pthread_mutex_destroy(&mutex);
	printf("code = %d\n", status);
	printf("programming is end\n");
	exit(0);
}
void *ssu_loop1(void *arg)
{
	int i;

	for (i=0;i<10;i++)
	{
		/*lock the critical section*/
		pthread_mutex_lock(&mutex);
		printf("loop1 : %d\n", shared_value);
		/*increase the value*/
		shared_value++;
		/*if i reaches 10 exit*/
		if (i == 10)
			return NULL;
		/*unlock the critical section*/
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	return NULL;
}
void *ssu_loop2(void *arg)
{
	int i;
	for (i=0;i<10;i++)
	{
		/*lock the critical section*/
		pthread_mutex_lock(&mutex);
		printf("loop2 : %d\n", shared_value);
		/*increase the value*/
		shared_value++;
		/*unlock the critical section*/
		pthread_mutex_unlock(&mutex);
		sleep(2);
	}
	return NULL;
}
