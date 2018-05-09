#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

void todo()
{
	int cnt=0;
	int fd = open("/home/jahn/lsp/p/ssu_backup/log.txt",
			O_RDWR|O_CREAT|O_TRUNC, 0644);
	char buf[10];
	while (1)
	{
		sleep(1);
		sprintf(buf, "%d",cnt);
		write(fd,buf,strlen(buf));
		write(fd, "\n", 1);
		cnt++;
		if (cnt>30)
			break;
	}
	close(fd);
	exit(0);
}

int daemon_init(void)
{
	pid_t pid;
	int fd, maxfd;
	//1)백그라운드 프로세스 분기
	if ((pid = fork()) < 0)
	{
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	//부모 종료
	else if (pid!=0)
		exit(0);
	pid = getpid();
	//2)세션 생성, 
	setsid();
	//3)작업제어 시그널 무시
	signal(SIGTTIN, SIG_IGN);	//제어 터미널 읽기 무시
	signal(SIGTTOU, SIG_IGN);	//제어터미널 쓰기 무시
	signal(SIGTSTP, SIG_IGN);	//터미널 일시중지 무시
	//4)오픈된 모든 디스크립터 닫기
	maxfd = getdtablesize();
	for (fd = 0;fd < maxfd; fd++)
		close(fd);
	//5)파일모드생성마스크 해제
	umask(0);
	//6)작업디렉토리 루트로 이동
	chdir("/");
	//7)fd 0,1,2 null로 재지정
	fd = open("/dev/null", O_RDWR);
	dup(0);
	dup(0);
	todo();
	return 0;
}
