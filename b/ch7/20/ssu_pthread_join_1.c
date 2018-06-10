#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *ssu_thread(void *arg);

int main(void)
{
	pthread_t tid1, tid2;
	int thread1 = 1;
	int thread2 = 2;
	void *status;

	/*create new thread*/
	if (pthread_create(&tid1, NULL, ssu_thread, (void *)&thread1) != 0)
	{
		fprintf(stderr, "pthread_create error\n");
		exit(1);
	}

	/*create new thread*/
	if (pthread_create(&tid2, NULL, ssu_thread, (void *)&thread2) != 0)
	{
		fprintf(stderr, "pthread_create error\n");
		exit(1);
	}
	/*wait for thread 1 to exit*/
	pthread_join(tid1, (void *)&status);
	/*wait for thread 2 to exit*/
	pthread_join(tid2, (void *)&status);
	exit(0);
}

void *ssu_thread(void *arg)
{
	int thread_index;
	int i;

	thread_index = *((int *)arg);

	/*print 0~4*/
	for (i=0;i<5;i++)
	{
		printf("%d : %d\n", thread_index, i);
		sleep(1);
	}

	return NULL;
}
