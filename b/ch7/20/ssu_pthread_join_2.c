#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *ssu_thread1(void *arg);
void *ssu_thread2(void *arg);

int main(void)
{
	pthread_t tid1, tid2;

	if (pthread_create(&tid1, NULL, ssu_thread1, NULL) != 0)
	{
		fprintf(stderr, "pthread_create error\n");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, ssu_thread2, NULL) != 0)
	{
		fprintf(stderr, "pthread_create error\n");
		exit(1);
	}

	printf("thread1의 리턴을 기다림\n");
	pthread_join(tid1, NULL);
	exit(0);
}

void *ssu_thread1(void *arg)
{
	int i;

	for (i=5;i != 0;i--)
	{
		printf("thread1: %d\n", i);
		sleep(1);
	}

	printf("thread1 complete\n");
	return NULL;
}

void *ssu_thread2(void *arg)
{
	int i;

	for (i=8;i != 0;i--)
	{
		printf("thread2: %d\n", i);
		sleep(1);
	}

	printf("thread2 complete\n");
	return NULL;
}
