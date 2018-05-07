#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

struct ssu_id
{
	char name[64];
	int id;
};

int main(void)
{
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	struct ssu_id test1, test2;
	char *fname = "ssu_exam.dat";
	FILE *fp;
	//쓰기모드로 파일스트림 오픈
	if ((fp = fopen(fname, "w")) == NULL)
	{
		fprintf(stderr, "fopen error %s\n", fname);
		exit(1);
	}
	//ID입력
	printf("Input ID>> ");
	scanf("%d", &test1.id);
	//이름입력
	printf("Input name>> ");
	scanf("%s", test1.name);
	//구조체내용 파일에 쓰기
	if(fwrite(&test1, sizeof(struct ssu_id), 1, fp) != 1)
	{
		fprintf(stderr, "fwrite error\n");
		exit(1);
	}

	fclose(fp);
	//읽기모드로 파일스트림 오픈
	if ((fp = fopen(fname, "r")) == NULL)
	{
		fprintf(stderr, "fopen error for %s\n" ,fname);
		exit(1);
	}
	//구조체 크기만큼 읽어서 test2에 저장
	if (fread(&test2, sizeof(struct ssu_id), 1, fp) != 1)
	{
		fprintf(stderr, "fread error\n");
		exit(1);
	}
	//구조체내용 출력
	printf("\nID	name\n");
	printf(" ============\n");
	printf("%d		%s\n", test2.id, test2.name);
	fclose(fp);
	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",(end.tv_sec-begin.tv_sec)*1000000 + end.tv_usec-begin.tv_usec); 
	exit(0);
}

