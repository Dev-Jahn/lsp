#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(void)
{
	char buf[BUFFER_SIZE];
	int length;

	//표준입력으로 부터 읽은 값을 버퍼에 저장, 길이를 length에 저장
	length = read(0, buf, BUFFER_SIZE);
	//표준출력에 버퍼의 내용을 길이만큼 쓰기
	write(1, buf, length);
	exit(0);
}
