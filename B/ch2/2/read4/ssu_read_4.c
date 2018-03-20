#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define TABLE_SIZE 128
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
	static struct
	{
		long offset;
		int length;
	} table [TABLE_SIZE];
	char buf[BUFFER_SIZE];
	long offset;
	int entry;
	int i;
	int length;
	int fd;

	//인자가 부족할때
	if (argc<2)
	{
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		exit(1);
	}
	//첫번째 인자의 파일을 읽기로 열어 fd에 할당
	if ((fd = open(argv[1], O_RDONLY))<0)
	{
		fprintf(stderr, "open error for %s\n", argv[1]);
		exit(1);
	}
	//변수 초기화
	entry = 0;
	offset = 0;
	//1024바이트 읽어 버퍼에 저장
	while ((length = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		//한바이트 씩 순회
		for (i = 0;i<length;i++)
		{
			//한줄이 끝나기 전까지 length와 offset증가
			table[entry].length++;
			offset++;
			//한줄이 끝나면 테이블 인덱스 증가, offset 저장
			if (buf[i] == '\n')
				table[++entry].offset = offset;
		}
	}

#ifdef DEBUG
	//테이블의 모든 내용 출력
	for (i=0;i<entry;i++)
		printf("%d: %ld, %d\n", i+1, table[i].offset, table[i].length);
#endif
	//사용자가 중단하기 전까지 반복
	while(1)
	{
		printf("Enter line number: ");
		scanf("%d", &length);

		//
		if(--length<0)
			break;
		//입력받은 행인덱스까지 오프셋 이동
		lseek(fd, table[length].offset, 0);
		//length만큼 읽어 버퍼에 저장
		//읽은 내용이 없으면 다시 행 입력받기
		if (read(fd, buf, table[length].length)<=0)
			continue;
		//문자열 끝에 널 삽입
		buf[table[length].length]='\0';
		//출력
		printf("%s", buf);
	}
	close(fd);
	exit(0);
}
