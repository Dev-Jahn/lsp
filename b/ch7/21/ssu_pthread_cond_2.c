#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int glo_val1 = 1, glo_val2 = 2;

void *ssu_thread1(void *arg);
void *ssu_thread2(void *arg);

int main(void)
{
	pthread_t tid1, tid2;

	/*create new threads*/
	pthread_create(&tid1, NULL, ssu_thread1, NULL);
	pthread_create(&tid2, NULL, ssu_thread2, NULL);
	/*wait for threads to exit*/
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	/*free the mutex variable*/
	pthread_mutex_destroy(&lock);
	/*free the conditional variable*/
	pthread_cond_destroy(&cond);
	exit(0);
}

void *ssu_thread1(void *arg)
{
	sleep(1);
	glo_val1 = 2;
	glo_val2 = 1;

	/*send signal to all threads*/
	if (glo_val1>glo_val2)
		pthread_cond_broadcast(&cond);
	printf("ssu_thread1 end\n");
	return NULL;
}

void *ssu_thread2(void *arg)
{
	struct timespec timeout;
	struct timeval now;

	/*lock the mutex*/
	pthread_mutex_lock(&lock);
	gettimeofday(&now, NULL);

	timeout.tv_sec = now.tv_sec +5;
	timeout.tv_nsec = now.tv_usec*1000;
	if(glo_val1<=glo_val2)
	{
		printf("ssu_thread2 sleep\n");
		/*if there's no signal within 5secs, print timeout*/
		if (pthread_cond_timedwait(&cond, &lock, &timeout) == ETIMEDOUT)
			printf("timeout\n");
		/*if there's signal within 5secs print values*/
		else
			printf("glo_val1 = %d, glo_val2 = %d\n", glo_val1, glo_val2);
	}
	/*unlock the mutex*/
	pthread_mutex_unlock(&lock);
	printf("ssu_thread2 end\n");
	return NULL;
}
