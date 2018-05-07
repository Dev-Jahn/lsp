#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

struct ssu_pirate
{
	unsigned long booty;
	unsigned int beard_length;
	char name[128];
};

int main(void)
{
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	struct ssu_pirate blackbeard = {950, 48, "Edward Teach"}, pirate;
	char *fname = "ssu_data";
	FILE *fp1, *fp2;
	//쓰기모드로 파일스트림 열기
	if ((fp2 = fopen(fname, "w")) == NULL)
	{
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}
	//구조체 내용 파일에 쓰기
	if (fwrite(&blackbeard, sizeof(struct ssu_pirate), 1, fp2) != 1)
	{
		fprintf(stderr, "fwrite error\n");
		exit(1);
	}
	//닫기
	if (fclose(fp2))
	{
		fprintf(stderr, "fclose error\n");
		exit(1);
	}
	//읽기모드로 파일스트림 열기	
	if ((fp1 = fopen(fname, "r")) == NULL)
	{
		fprintf(stderr, "fopen error\n");
		exit(1);
	}
	//구조체크기만큼 읽어서 pirate구조체에 저장
	if (fread(&pirate, sizeof(struct ssu_pirate), 1, fp1) != 1)
	{
		fprintf(stderr, "fread error\n");
		exit(1);
	}

	if (fclose(fp1))
	{
		fprintf(stderr, "fclose error\n");
		exit(1);
	}

	printf("name=\"%s\" booty=%lu beard_length=%u\n", pirate.name, pirate.booty, pirate.beard_length);
	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",(end.tv_sec-begin.tv_sec)*1000000 + end.tv_usec-begin.tv_usec); 
	exit(0);
}
