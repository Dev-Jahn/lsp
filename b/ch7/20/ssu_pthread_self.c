#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *ssu_thread(void *arg);

int main(void)
{
	pthread_t tid;

	/*create new thread*/
	if (pthread_create(&tid, NULL, ssu_thread, NULL) != 0)
	{
		fprintf(stderr, "pthread_create error\n");
		exit(1);
	}

	/*print tid of created thread*/
	printf("%u\n", (unsigned int)tid);

	/*create new thread*/
	if (pthread_create(&tid, NULL, ssu_thread, NULL) != 0)
	{
		fprintf(stderr, "pthread_create error\n");
		exit(1);
	}

	/*print tid of created thread*/
	printf("%u\n", (unsigned int)tid);
	sleep(1);
	exit(0);
}

void *ssu_thread(void *arg)
{
	pthread_t tid;

	/*print tid of this thread*/
	tid = pthread_self();
	printf("->%u\n", (unsigned int)tid);
	return NULL;
}

