#include <stdio.h>
int main()
{
	char buf[100] = {0};
	fgets(buf,100,stdin);
	printf("%s",buf);
	/*ungetc('A',stdin);*/
	/*ungetc('B',stdin);*/
	/*ungetc('C',stdin);*/
	/*
	 *for (int i=0;i<100;i++)
	 *    printf("%c,",stdin->_IO_read_ptr[i-5]);
	 *printf("\n");
	 */
	/*
	 *getchar();
	 *for (int i=0;i<100;i++)
	 *    printf("%c,",stdin->_IO_read_ptr[i]);
	 *printf("\n");
	 */
}

