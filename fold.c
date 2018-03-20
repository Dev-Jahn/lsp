#include <stdio.h>

void func()
{
	int a = 2;
	int c = 55;
}

int main()
{
	int a = 1;
	int b = 3;
	while(b--)
	{
		if (a)
		{
			func();
		}
	}
}
