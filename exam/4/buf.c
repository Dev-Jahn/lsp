#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define SIZ 100
char buf[SIZ];
void print(FILE *fp, int i)
{
	fprintf(fp, "word%d\n",i);
	for (int i=0;i<SIZ;i++)
		fprintf(stderr,"%c,",buf[i]);
	fprintf(stderr, "\n\n");
	usleep(300000);
}
int main(void)
{
	FILE *fp = fopen("buf.txt","w+");
	setvbuf(fp, buf, _IOFBF, SIZ);
	for (int i=0;i<100;i++)
		print(fp,i);
	sleep(1);
	/*fflush(fp);*/
	fclose(fp);
}
