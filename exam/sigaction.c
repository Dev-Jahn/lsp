#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int signo)
{

}
void (*ssu_func)(int);

int main(void)
{
	ssu_func = signal(SIGINT, handler);
}
