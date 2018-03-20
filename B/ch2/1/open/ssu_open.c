#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

 status 1을 리턴하며 종료
		exit(1);
	}
	else
		//표준출력으로 성공메시지 출력
		printf("Success!\nFilename : %s\nDescriptor : %d\n", fname, fd);
	//종료
	exit(0);
}
