#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void &ssu_thread(void *arg);

int main(void)
{
	pthread_t tid;
	pid_t pid;

	if (pthread_create(&tid, NULL, ssu_thread, NULL) !=0)
	{
		fprintf(stderr, "pthread_create error\n");
		exit(1);
	}
	pid = getpid();
	tid = pthread_self;
	printf("Main Thread: pid %
