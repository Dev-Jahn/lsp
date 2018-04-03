#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssu_employee.h"

int main(int argc, char *argv[])
{
	struct ssu_employee record;
	int fd;
	int record_num;
	//인자가 없을때
	if (argc <2)
	{
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		exit(1);
	}

	//읽기전용으로 파일 오픈
	if ((fd=open(argv[1], O_RDONLY)) < 0)
	{
		fprintf(stderr, "open error for %s\n", argv[1]);
		exit(1);
	}

	//사용자가 종료 전까지 반복
	while(1)
	{
		//읽을 구조체 번호를 입력
		printf("Enter record number :" );
		scanf("%d", &record_num);

		//0보다 작으면 탈출
		if (record_num < 0)
			break;

		//읽을 구조체까지 오프셋이동
		if (lseek(fd, (long)record_num * sizeof(record), 0) < 0)
		{
			fprintf(stderr, "lseek error\n");
			exit(1);
		}

		//구조체 크기만큼 읽어서 구조체 변수에 저장, 내용 출력
		if (read(fd, (char *)&record, sizeof(record)) > 0)
			printf("Employee: %s Salary : %d\n", record.name, record.salary);
		else
			printf("Record %d not found\n", record_num);
	}

	close(fd);
	exit(0);
}
