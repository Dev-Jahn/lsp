#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *thread1(void *arg);
void *thread2(void *arg);

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;

int input, count, arr1[1000], arr2[1000];

int main(void)
{
	pthread_t tid1, tid2;

	pthread_create(&tid1, NULL, thread1, NULL);
	pthread_create(&tid2, NULL, thread2, NULL);
	while (1)
	{
		printf("enter number > ");
		scanf("%d", &input);

		if (input>=1)
		{
			pthread_cond_signal(&cond1);
			break;
		}
	}
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	printf("end\n");
	exit(0);
}

void *thread1(void *arg)
{
	while (1)
	{
		pthread_mutex_lock(&mutex1);

		if (input <1)
			pthread_cond_wait(&cond1, &mutex1);
		if (input == count)
		{
			pthread_cond_signal(&cond2);
			break;
		}

		if (count%2 == 0)
		{
			for (int i=0;i<count/2+1;i++)
			{
				if (i==0)
					arr1[i] = 1;
				else
					arr1[i] = arr2[i-1]+arr2[i];
			}
			for (int i=0;i<count/2+1;i++)
				arr1[count-i] = arr1[i];
			count++;
		}

		printf("thread1: ");
		for (int i=0;i<count;i++)
			printf("%d ", arr1[i]);
		printf("\n");

		pthread_cond_signal(&cond2);
		pthread_cond_wait(&cond1, &mutex1);

		pthread_mutex_unlock(&mutex1);
	}

	return NULL;
}

void *thread2(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex2);

		if (input <1)
			pthread_cond_wait(&cond2, &mutex2);

		if (input == count)
		{
			pthread_cond_signal(&cond1);
			break;
		}
		if (count%2 == 1)
		{
			for (int i=0;i<count/2+1;i++)
			{
				if (i==0)
					arr2[i] = 1;
				else
					arr2[i] = arr1[i-1]+arr1[i];
			}
			for (int i=0;i<count/2+1;i++)
				arr2[count-i] = arr2[i];
			count++;
		}

		printf("thread2: ");
		for (int i=0;i<count;i++)
			printf("%d ", arr2[i]);
		printf("\n");

		pthread_cond_signal(&cond1);
		pthread_cond_wait(&cond2, &mutex2);

		pthread_mutex_unlock(&mutex2);
	}
	return NULL;
}
