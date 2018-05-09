#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int ssu_daemon_init(void);

int main(void)
{
	pid_t pid;
	//현재 프로세스ID
	pid = getpid();
	//pid 출력
	printf("parent process : %d\n", pid);
	printf("daemon process initialization\n");
	//daemon 프로세스 초기화
	if (ssu_daemon_init() < 0)
	{
		fprintf(stderr, "ssu_daemon_init failed\n");
		exit(1);
	}

	int cnt=0;
	int fd = open("/home/jahn/lsp/p/test/log.txt",O_RDWR|O_CREAT|O_TRUNC, 0644);
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

int ssu_daemon_init(void)
{
	pid_t pid;
	int fd, maxfd;
	//프로세스 분기
	if ((pid = fork()) < 0)
	{
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	//부모 종료
	else if (pid!=0)
		exit(0);
	//자식 pid
	pid = getpid();
	//pid 출력
	printf("process %d running as daemon\n", pid);
		//세션 생성, 
	setsid();
	//작업제어 시그널 무시
	//제어 터미널 읽기 무시
	signal(SIGTTIN, SIG_IGN);
	//제어터미널 쓰기 무시
	signal(SIGTTOU, SIG_IGN);
	//터미널 일시중지 무시
	signal(SIGTSTP, SIG_IGN);
	//
	maxfd = getdtablesize();

	for (fd = 0;fd < maxfd; fd++)
		close(fd);
	umask(0);
	chdir("/");
	fd = open("/dev/null", O_RDWR);
	dup(0);
	dup(0);
	return 0;
}
