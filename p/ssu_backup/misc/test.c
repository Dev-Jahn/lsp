#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include "util.h"

pthread_t tids[1000];
size_t tcnt = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *func(void *arg);

int main(void)
{
	int fd;
	fd = open("testlog", O_WRONLY|O_TRUNC|O_CREAT, 0666);
	for (int i=0;i<10;i++)
	{
		pthread_create(&tids[tcnt], NULL, func, &fd);
		tcnt++;
	}
	for (size_t i=0;i<tcnt;i++)
		pthread_join(tids[i], NULL);
	close(fd);
}

void *func(void *arg)
{
	char buf[10] = {'a','b','c','d','e','f','g','h','i','j'};
	int fd = *(int*)arg;
	pthread_mutex_lock(&mutex);
	for (int i=0;i<10;i++)
		write(fd, buf+i, 1);
	write(fd, "\n", 1);
	pthread_mutex_unlock(&mutex);
	return NULL;
}
