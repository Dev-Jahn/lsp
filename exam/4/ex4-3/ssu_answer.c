#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 256
#define STUDENT_NUM 3
#define Q_SIZE 10

typedef struct _student {
	char name[10];
	int score;
	char res[BUFFER_SIZE];
} Student;

char answer[BUFFER_SIZE]="1233423413";	//정답

int main(void)
{
	char *ssu_answer = "ssu_answer.txt";
	char *ssu_res = "ssu_res.txt";
	char tmp_score[BUFFER_SIZE];
	FILE *fp;
	int i, j=0;
	Student list[STUDENT_NUM];

	//답안 파일 열기
	if ((fp = fopen(ssu_answer,"rb"))==NULL)
	{
		fprintf(stderr, "fopen error\n");
		exit(1);
	}
	//0~2
	for (j=0;j<STUDENT_NUM;j++)
	{
		list[j].score = 0;
		//한줄읽어 name에 저장
		if(fgets(list[j].name, BUFFER_SIZE, fp)==NULL)
		{
			fprintf(stderr, "fgets error\n");
			exit(1);
		}

		//이름끝에 널문자붙이기
		i=0;
		while((list[j].name[i]!='\n'))
			i++;
		list[j].name[i]='\0';
		
		/*
		 *소스수정
		 */
		if (fgets(list[j].res, BUFFER_SIZE, fp)==NULL)
		{
			fprintf(stderr, "fgets error\n");
			exit(1);
		}
		/*
		 *소스수정
		 */
		//결과 끝에 널문자 붙이기
		i=0;
		while((list[j].res[i]!='\n')) i++;
		list[j].res[i]='\0';
		//0~9
		for(i=0;i<Q_SIZE;i++)
		{
			if(list[j].res[i] ==answer[i])
			{
				list[j].score+=10;
				list[j].res[i]='0';
			}
			else
				list[j].res[i] = 'X';
		}
		printf("Student name:%s, score:%d, res:%s\n", list[j].name, list[j].score, list[j].res);
	}
	fclose(fp);

	if ((fp = fopen(ssu_res, "wb"))==NULL)
	{
		fprintf(stderr, "fopen error\n");
		exit(1);
	}
	for (i=0;i<STUDENT_NUM;i++)
	{
		if(fputs(list[i].name, fp)==EOF)
		{
			fprintf(stderr, "fputs error\n");
			exit(1);
		}

		sprintf(tmp_score, " |%d| ", list[i].score);
		if (fputs(tmp_score, fp) == EOF)
		{
			fprintf(stderr, "fputs error\n");
			exit(1);
		}

		if(fputs(list[i].res,fp) == EOF)
		{
			fprintf(stderr, "fputs error\n");
			exit(1);
		}
	}
	fclose(fp);
	exit(0);
}
