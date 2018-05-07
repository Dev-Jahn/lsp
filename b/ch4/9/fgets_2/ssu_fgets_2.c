#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define BUFFER_MAX 256

int main(void)
{
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	char command[BUFFER_MAX];
	char *prompt = "Prompt>>";
	//무한반복
	while(1)
	{
		//프롬프트 출력
		fputs(prompt, stdout);
		//표준입력에서 한줄 입력, NULL이면 중단, 아니면 반복
		if (fgets(command, sizeof(command), stdin) == NULL)
			break;
		//시스템함수 콜
		system(command);
	}
	//NULL입력시 문구 출력후 종료
	fprintf(stdout, "Good bye...\n");
	fflush(stdout);
	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",(end.tv_sec-begin.tv_sec)*1000000 + end.tv_usec-begin.tv_usec); 
	exit(0);
}
